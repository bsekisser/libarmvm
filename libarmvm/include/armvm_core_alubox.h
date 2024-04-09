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

typedef uint32_t (*alu_fn)(uint32_t sop, armvm_core_p core);

static void __alubox__fn_wb(uint32_t sop, alu_fn fn, armvm_core_p core)
{ arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), fn(sop, core)); }

/* **** */

static uint32_t _alubox_flags__nz(uint32_t result, armvm_core_p core)
{
	if(CCX) {
		ARM_CPSR_BMAS(N, BEXT(result, 31));
		ARM_CPSR_BMAS(Z, 0 == result);
	}

	return(result);
}

static uint32_t _alubox_flags__nzc(uint32_t result, armvm_core_p core)
{
	if(CCX)
		ARM_CPSR_BMAS(C, _shifter_operand_c(core));

	return(_alubox_flags__nz(result, core));
}

/* **** */

static uint32_t _alubox_nzc(uint32_t sop, alu_fn fn, armvm_core_p core)
{
	const uint32_t result = fn(sop, core);
	_alubox_flags__nzc(result, core);

	return(result);
}

static void _alubox_nzc_wb(uint32_t sop, alu_fn fn, armvm_core_p core)
{
	uint32_t result = arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), fn(sop, core));

	if(CCX && rR_IS_PC(D)) {
		if(pSPSR)
			CPSR = spsr(0, core);
	} else
		_alubox_flags__nzc(result, core);
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

static uint32_t _alubox_flags_add_sub(uint32_t rd, uint32_t s1, uint32_t s2, armvm_core_p core)
{
	_alubox_flags__nz(rd, core);

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

static uint32_t _alubox_flags_adds(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_core_p core)
{
	uint32_t result = rn + sop + carry_in;

	return(_alubox_flags_add_sub(result, rn, sop, core));
}

static void _alubox_flags_adds_wb(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_core_p core)
{
	const uint32_t rd = _alubox_flags_adds(rn, sop, carry_in, core);
	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);
}

static uint32_t _alubox_flags_subs(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_core_p core)
{
	const uint32_t result = rn - sop - carry_in;

	return(_alubox_flags_add_sub(result, -rn, sop, core));
}

static void _alubox_flags_subs_wb(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_core_p core)
{
	const uint32_t rd = _alubox_flags_subs(rn, sop, carry_in, core);
	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);
}

/* **** */

static uint32_t alubox_adc(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn + sop + (IF_CPSR(C) ? 1 : 0));
}

static void alubox_adc_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_adc, core)); }

static void alubox_adcs(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_adc_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(rn, sop, IF_CPSR(C), core));
}

static uint32_t alubox_add(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	return(rn + sop);
}

static void alubox_add_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_add, core)); }

static void alubox_adds(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_add_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(rn, sop, 0, core));
}

static uint32_t alubox_and(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn & sop);
}

static void alubox_and_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_and, core)); }

static void alubox_ands(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_and, core)); }

static uint32_t alubox_bic(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn & ~sop);
}

static void alubox_bic_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_bic, core)); }

static void alubox_bics(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_bic, core)); }

static uint32_t alubox_eor(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn ^ sop);
}

static void alubox_eor_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_eor, core)); }

static void alubox_eors(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_eor, core)); }

static uint32_t alubox_mov(uint32_t sop, armvm_core_p core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(sop);
}

static void alubox_mov_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_mov, core)); }

static void alubox_movs(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_mov, core)); }

static uint32_t alubox_mvn(uint32_t sop, armvm_core_p core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(~sop);
}

static void alubox_mvn_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_mvn, core)); }

static void alubox_mvns(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_mvn, core)); }

static uint32_t alubox_orr(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn | sop);
}

static void alubox_orr_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_orr, core)); }

static void alubox_orrs(uint32_t sop, armvm_core_p core)
{ return(_alubox_nzc_wb(sop, alubox_orr, core)); }

static uint32_t alubox_rsb(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn);
}

static void alubox_rsb_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_rsb, core)); }

static void alubox_rsbs(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_rsb_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(sop, rn, 0, core));
}

static uint32_t alubox_rsc(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn - (IF_CPSR(C) ? -1 : 0));
}

static void alubox_rsc_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_rsc, core)); }

static void alubox_rscs(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_rsc_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(sop, rn, carry_in, core));
}

static uint32_t alubox_sbc(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(rn - sop - (IF_CPSR(C) ? -1 : 0));
}

static void alubox_sbc_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_sbc, core)); }

static void alubox_sbcs(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_sbc_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(rn, sop, carry_in, core));
}

static uint32_t alubox_sub(uint32_t sop, armvm_core_p core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	LOG("rn: 0x%08x, sop: 0x%08x", rn, sop);

	return(rn - sop);
}

static void alubox_sub_wb(uint32_t sop, armvm_core_p core)
{ return(__alubox__fn_wb(sop, alubox_sub, core)); }

static void alubox_subs(uint32_t sop, armvm_core_p core)
{
	if(rR_IS_PC(D)) {
		alubox_sub_wb(sop, core);
		if(CCX && pSPSR)
			CPSR = spsr(0, core);

		return;
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(rn, sop, 0, core));
}

static void alubox_teqs(uint32_t sop, armvm_core_p core)
{ _alubox_nzc(sop, alubox_eor, core); }

static void alubox_tsts(uint32_t sop, armvm_core_p core)
{ _alubox_nzc(sop, alubox_and, core); }

/* **** */

static void alubox_cmns(uint32_t sop, armvm_core_p core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_adds(rn, sop, 0, core);
}

static void alubox_cmps(uint32_t sop, armvm_core_p core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_subs(rn, sop, 0, core);
}
