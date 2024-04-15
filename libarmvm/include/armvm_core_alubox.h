#include "armvm_core_glue.h"
#include "armvm_core_reg_trace.h"
#include "armvm_core_shiftbox.h"

/* **** */

#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"

/* **** */

#include "libbse/include/bitfield.h"
//#include git/libbse/shift_roll.h"

/* **** */

#include <assert.h>

/* **** */

typedef uint32_t (*alu_fn)(armvm_core_p const core, const uint32_t sop);

static void __alubox__fn_wb(armvm_core_p const core, const uint32_t sop, alu_fn const fn)
{ arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), fn(core, sop)); }

/* **** */

static uint32_t _alubox_flags__nz(armvm_core_p const core, const uint32_t result)
{
	if(CCX) {
		ARM_CPSR_BMAS(N, BEXT(result, 31));
		ARM_CPSR_BMAS(Z, 0 == result);
	}

	return(result);
}

static uint32_t _alubox_flags__nzc(armvm_core_p const core, const uint32_t result)
{
	if(CCX)
		ARM_CPSR_BMAS(C, _shifter_operand_c(core));

	return(_alubox_flags__nz(core, result));
}

/* **** */

static uint32_t _alubox_nzc(armvm_core_p const core, const uint32_t sop, alu_fn const fn)
{
	const uint32_t result = fn(core, sop);
	_alubox_flags__nzc(core, result);

	return(result);
}

static void _alubox_nzc_wb(armvm_core_p const core, const uint32_t sop, alu_fn const fn)
{
	uint32_t result = arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), fn(core, sop));

	if(CCX && rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);
	} else
		_alubox_flags__nzc(core, result);
}

/*
 * Credit to:
 * 		http://www.emulators.com/docs/nx11_flags.htm
 *
 * OF(A+B) = ((A XOR D) AND NOT (A XOR B)) < 0
 * OF(A-B) = ((A XOR D) AND (A XOR B)) < 0
 *
 * CF(A+B) = (((A XOR B) XOR D) < 0) XOR (((A XOR D) AND NOT (A XOR B)) < 0)
 * CF(A-B) = (((A XOR B) XOR D) < 0) XOR (((A XOR D) AND (A XOR B)) < 0)
 *
 */

static uint32_t _alubox_flags_add_sub(armvm_core_p const core, const uint32_t rd,
	const uint32_t s1, const uint32_t s2)
{
	_alubox_flags__nz(core, rd);

	const unsigned xvec = (s1 ^ s2);
	const unsigned ovec = (s1 ^ rd) & ~xvec;

	const unsigned cf = BEXT((xvec ^ ovec ^ rd), 31);
	ARM_CPSR_BMAS(C, cf);

	const unsigned vf = BEXT(ovec, 31);
	ARM_CPSR_BMAS(V, vf);

	if(0) LOG("rd: 0x%08x, s1: 0x%08x, s2: 0x%08x, C: %01u, V: %01u",
		rd, s1, s2, cf, vf);

	return(rd);
}

static uint32_t _alubox_flags_adds(armvm_core_p const core, const uint32_t rn,
	const uint32_t sop, const uint32_t carry_in)
{
	uint32_t result = rn + sop + carry_in;

	return(_alubox_flags_add_sub(core, result, rn, sop));
}

static void _alubox_flags_adds_wb(armvm_core_p const core, const uint32_t rn,
	const uint32_t sop, const uint32_t carry_in)
{
	const uint32_t rd = _alubox_flags_adds(core, rn, sop, carry_in);
	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);
}

static uint32_t _alubox_flags_subs(armvm_core_p const core, const uint32_t rn,
	uint32_t sop, uint32_t carry_in)
{
	const uint32_t result = rn - sop - carry_in;

	return(_alubox_flags_add_sub(core, result, -rn, sop));
}

static void _alubox_flags_subs_wb(armvm_core_p const core, const uint32_t rn,
	const uint32_t sop, const uint32_t carry_in)
{
	const uint32_t rd = _alubox_flags_subs(core, rn, sop, carry_in);
	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);
}

/* **** */

static uint32_t alubox_adc(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn + sop + (IF_CPSR(C) ? 1 : 0));
}

static void alubox_adc_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_adc)); }

static void alubox_adcs(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_adc_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(core, rn, sop, IF_CPSR(C)));
}

static uint32_t alubox_add(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	return(rn + sop);
}

static void alubox_add_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_add)); }

static void alubox_adds(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_add_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(core, rn, sop, 0));
}

static uint32_t alubox_and(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn & sop);
}

static void alubox_and_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_and)); }

static void alubox_ands(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_and)); }

static uint32_t alubox_bic(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn & ~sop);
}

static void alubox_bic_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_bic)); }

static void alubox_bics(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_bic)); }

static uint32_t alubox_eor(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn ^ sop);
}

static void alubox_eor_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_eor)); }

static void alubox_eors(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_eor)); }

static uint32_t alubox_mov(armvm_core_p const core, const uint32_t sop)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(sop);
}

static void alubox_mov_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_mov)); }

static void alubox_movs(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_mov)); }

static uint32_t alubox_mvn(armvm_core_p const core, const uint32_t sop)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(~sop);
}

static void alubox_mvn_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_mvn)); }

static void alubox_mvns(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_mvn)); }

static uint32_t alubox_orr(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn | sop);
}

static void alubox_orr_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_orr)); }

static void alubox_orrs(armvm_core_p const core, const uint32_t sop)
{ return(_alubox_nzc_wb(core, sop, alubox_orr)); }

static uint32_t alubox_rsb(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn);
}

static void alubox_rsb_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_rsb)); }

static void alubox_rsbs(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_rsb_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(core, sop, rn, 0));
}

static uint32_t alubox_rsc(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn - (IF_CPSR(C) ? -1 : 0));
}

static void alubox_rsc_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_rsc)); }

static void alubox_rscs(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_rsc_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(core, sop, rn, carry_in));
}

static uint32_t alubox_sbc(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn - sop - (IF_CPSR(C) ? -1 : 0));
}

static void alubox_sbc_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_sbc)); }

static void alubox_sbcs(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_sbc_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(core, rn, sop, carry_in));
}

static uint32_t alubox_sub(armvm_core_p const core, const uint32_t sop)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn - sop);
}

static void alubox_sub_wb(armvm_core_p const core, const uint32_t sop)
{ return(__alubox__fn_wb(core, sop, alubox_sub)); }

static void alubox_subs(armvm_core_p const core, const uint32_t sop)
{
	if(rR_IS_PC(D)) {
		alubox_sub_wb(core, sop);
		if(CCX && pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(core, rn, sop, 0));
}

static void alubox_teqs(armvm_core_p const core, const uint32_t sop)
{ _alubox_nzc(core, sop, alubox_eor); }

static void alubox_tsts(armvm_core_p const core, const uint32_t sop)
{ _alubox_nzc(core, sop, alubox_and); }

/* **** */

static void alubox_cmns(armvm_core_p const core, const uint32_t sop)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_adds(core, rn, sop, 0);
}

static void alubox_cmps(armvm_core_p const core, const uint32_t sop)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_subs(core, rn, sop, 0);
}
