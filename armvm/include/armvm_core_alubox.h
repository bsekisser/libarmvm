#include "armvm_core_glue.h"
#include "armvm_core_reg_trace.h"
#include "armvm_core_shiftbox.h"

/* **** */

#include "arm_cpsr.h"
#include "arm_ir.h"
#include "armvm.h"

/* **** */

#include "bitfield.h"
//#include "shift_roll.h"

/* **** */

#include <assert.h>

/* **** */

typedef uint32_t (*alu_fn)(uint32_t sop, armvm_p avm);

void __alubox__fn_wb(uint32_t sop, alu_fn fn, armvm_p avm)
{ return(arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), fn(sop, avm))); }

/* **** */

uint32_t _alubox_flags__add_c(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t result = rn + sop + carry_in;

	if(CCX) {
		const uint64_t x_result = result;

		ARM_CPSR_BMAS(C, result != x_result);
	}

	return(result);
}

uint32_t _alubox_flags__add_v(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const int32_t result = rn + sop + carry_in;

	if(CCX) {
		const int64_t x_result = result;

		ARM_CPSR_BMAS(V, result != x_result);
	}

	return(result);
}

uint32_t _alubox_flags__sub_c(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t result = rn - sop - carry_in;

if(0) LOG("rn: 0x%08x, sop: 0x%08x, carry_in: 0x%08x, result: 0x%08x",
		rn, sop, carry_in, result);

	if(CCX) {
		const uint64_t x_result = result;

//		LOG("x_result: 0x%016" PRIx64, x_result);
if(0) LOG("x_result: 0x%016lx", x_result);

		ARM_CPSR_BMAS(C, result != x_result);
	}

	return(result);
}

uint32_t _alubox_flags__sub_v(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const int32_t result = rn - sop - carry_in;

if(0) LOG("rn: 0x%08x, sop: 0x%08x, carry_in: 0x%08x, result: 0x%08x",
		rn, sop, carry_in, result);

	if(CCX) {
		const int64_t x_result = result;

//		LOG("x_result: 0x%016" PRIx64, x_result);
if(0) LOG("x_result: 0x%016lx", x_result);

		ARM_CPSR_BMAS(V, result != x_result);
	}

	return(result);
}

uint32_t _alubox_flags__nz(uint32_t result, armvm_p avm)
{
	if(CCX) {
		ARM_CPSR_BMAS(N, BEXT(result, 31));
		ARM_CPSR_BMAS(Z, 0 == result);
	}

	return(result);
}

uint32_t _alubox_flags__nzc(uint32_t result, armvm_p avm)
{
	if(CCX)
		ARM_CPSR_BMAS(C, _shifter_operand_c(avm));

	return(_alubox_flags__nz(result, avm));
}

/* **** */

uint32_t _alubox_nzc(uint32_t sop, alu_fn fn, armvm_p avm)
{
	const uint32_t result = fn(sop, avm);
	_alubox_flags__nzc(result, avm);

	return(result);
}

void _alubox_nzc_wb(uint32_t sop, alu_fn fn, armvm_p avm)
{
	const uint32_t rd = _alubox_nzc(sop, fn, avm);
	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);

	if(CCX && rR_IS_PC(D)) {
		if(pSPSR)
			CPSR = SPSR;
	}
}

uint32_t _alubox_flags_adds(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t result = _alubox_flags__add_c(rn, sop, carry_in, avm);

	(void)_alubox_flags__add_v(rn, sop, carry_in, avm);
	_alubox_flags__nz(result, avm);

	return(result);
}

void _alubox_flags_adds_wb(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t rd = _alubox_flags_adds(rn, sop, carry_in, avm);
	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);
}

uint32_t _alubox_flags_subs(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t result = _alubox_flags__sub_c(rn, sop, carry_in, avm);

	(void)_alubox_flags__sub_v(rn, sop, carry_in, avm);
	_alubox_flags__nz(result, avm);

	return(result);
}

void _alubox_flags_subs_wb(uint32_t rn, uint32_t sop, uint32_t carry_in, armvm_p avm)
{
	const uint32_t rd = _alubox_flags_subs(rn, sop, carry_in, avm);
	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);
}

/* **** */

uint32_t alubox_adc(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn + sop + (IF_CPSR(C) ? 1 : 0));
}

void alubox_adc_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_adc, avm)); }

void alubox_adcs(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_adc_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(rn, sop, IF_CPSR(C), avm));
}

uint32_t alubox_add(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));
	return(rn + sop);
}

void alubox_add_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_add, avm)); }

void alubox_adds(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_add_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_adds_wb(rn, sop, 0, avm));
}

uint32_t alubox_and(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn & sop);
}

void alubox_and_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_and, avm)); }

void alubox_ands(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_and, avm)); }

uint32_t alubox_bic(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn & ~sop);
}

void alubox_bic_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_bic, avm)); }

void alubox_bics(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_bic, avm)); }

uint32_t alubox_eor(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn ^ sop);
}

void alubox_eor_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_eor, avm)); }

void alubox_eors(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_eor, avm)); }

uint32_t alubox_mov(uint32_t sop, armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(sop);
}

void alubox_mov_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_mov, avm)); }

void alubox_movs(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_mov, avm)); }

uint32_t alubox_mvn(uint32_t sop, armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return(~sop);
}

void alubox_mvn_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_mvn, avm)); }

void alubox_mvns(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_mvn, avm)); }

uint32_t alubox_orr(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn | sop);
}

void alubox_orr_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_orr, avm)); }

void alubox_orrs(uint32_t sop, armvm_p avm)
{ return(_alubox_nzc_wb(sop, alubox_orr, avm)); }

uint32_t alubox_rsb(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn);
}

void alubox_rsb_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_rsb, avm)); }

void alubox_rsbs(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_rsb_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(sop, rn, 0, avm));
}

uint32_t alubox_rsc(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(sop - rn - (IF_CPSR(C) ? -1 : 0));
}

void alubox_rsc_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_rsc, avm)); }

void alubox_rscs(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_rsc_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(sop, rn, carry_in, avm));
}

uint32_t alubox_sbc(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(rn - sop - (IF_CPSR(C) ? -1 : 0));
}

void alubox_sbc_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_sbc, avm)); }

void alubox_sbcs(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_sbc_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t carry_in = IF_CPSR(C) ? -1 : 0;
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(rn, sop, carry_in, avm));
}

uint32_t alubox_sub(uint32_t sop, armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));
	LOG("rn: 0x%08x, sop: 0x%08x", rn, sop);

	return(rn - sop);
}

void alubox_sub_wb(uint32_t sop, armvm_p avm)
{ return(__alubox__fn_wb(sop, alubox_sub, avm)); }

void alubox_subs(uint32_t sop, armvm_p avm)
{
	if(rR_IS_PC(D)) {
		alubox_sub_wb(sop, avm);
		if(CCX && pSPSR)
			CPSR = SPSR;

		return;
	}

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	return(_alubox_flags_subs_wb(rn, sop, 0, avm));
}

void alubox_teqs(uint32_t sop, armvm_p avm)
{ _alubox_nzc(sop, alubox_eor, avm); }

void alubox_tsts(uint32_t sop, armvm_p avm)
{ _alubox_nzc(sop, alubox_and, avm); }

/* **** */

void alubox_cmns(uint32_t sop, armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_adds(rn, sop, 0, avm);
}

void alubox_cmps(uint32_t sop, armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));

	_alubox_flags_subs(rn, sop, 0, avm);
}
