#include "armvm_core_glue.h"
#include "armvm_core_shiftbox.h"

/* **** */

#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#include "local/alu_t.h"
#include "local/alubox_flags_arm.h"
#include "local/core_reg.h"

/* **** */

#include <assert.h>

/* **** */

static uint32_t __alubox__fn_wb(armvm_core_ref core, alu_fn const fn)
{ return(core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), fn(core))); }

/* **** */

static void alubox__fn_nzc_wb(armvm_core_ref core, alu_fn const fn)
{
	__alubox__fn_wb(core, fn);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);
	} else
		_alubox_flags_nzc(core, vR(D));
}

/* **** */

static uint32_t alubox_adc(armvm_core_ref core)
{
	const uint32_t cf = IF_CPSR(C) ? 1 : 0;
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn + vR(SOP) + cf));
}

static void alubox_adc_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_adc); }

static void alubox_adcs(armvm_core_ref core)
{
	alubox_adc_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_add(core);
}

static uint32_t alubox_add(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	return((vR(D) = rn + vR(SOP)));
}

static void alubox_add_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_add); }

static void alubox_adds(armvm_core_ref core)
{
	alubox_add_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_add(core);
}

static uint32_t alubox_and(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn & vR(SOP)));
}

static void alubox_and_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_and); }

static void alubox_ands(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_and); }

static uint32_t alubox_bic(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn & ~vR(SOP)));
}

static void alubox_bic_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_bic); }

static void alubox_bics(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_bic); }

static uint32_t alubox_eor(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn ^ vR(SOP)));
}

static void alubox_eor_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_eor); }

static void alubox_eors(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_eor); }

static uint32_t alubox_mov(armvm_core_ref core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return((vR(D) = vR(SOP)));
}

static void alubox_mov_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_mov); }

static void alubox_movs(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_mov); }

static uint32_t alubox_mvn(armvm_core_ref core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RN);

	return((vR(D) = ~vR(SOP)));
}

static void alubox_mvn_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_mvn); }

static void alubox_mvns(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_mvn); }

static uint32_t alubox_orr(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn | vR(SOP)));
}

static void alubox_orr_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_orr); }

static void alubox_orrs(armvm_core_ref core)
{ alubox__fn_nzc_wb(core, alubox_orr); }

static uint32_t alubox_rsb(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = vR(SOP) - rn));
}

static void alubox_rsb_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_rsb); }

static void alubox_rsbs(armvm_core_ref core)
{
	alubox_rsb_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_sub(core);
}

static uint32_t alubox_rsc(armvm_core_ref core)
{
//	const uint32_t cf = IF_CPSR(C) ? -1U : 0;
	const uint32_t cf = IF_CPSR(C) ? ~0U : 0;
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = vR(SOP) - rn - cf));
}

static void alubox_rsc_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_rsc); }

static void alubox_rscs(armvm_core_ref core)
{
	alubox_rsc_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_sub(core);
}

static uint32_t alubox_sbc(armvm_core_ref core)
{
//	const uint32_t cf = IF_CPSR(C) ? -1U : 0;
	const uint32_t cf = IF_CPSR(C) ? ~0U : 0;
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn - vR(SOP) - cf));
}

static void alubox_sbc_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_sbc); }

static void alubox_sbcs(armvm_core_ref core)
{
	alubox_sbc_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_sub(core);
}

static uint32_t alubox_sub(armvm_core_ref core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	return((vR(D) = rn - vR(SOP)));
}

static void alubox_sub_wb(armvm_core_ref core)
{ __alubox__fn_wb(core, alubox_sub); }

static void alubox_subs(armvm_core_ref core)
{
	alubox_sub_wb(core);

	if(!CCX) return;

	if(rR_IS_PC(D)) {
		if(pSPSR)
			armvm_core_psr_mode_switch_cpsr_spsr(core);

		return;
	}

	alubox_flags_sub(core);
}

static void alubox_teqs(armvm_core_ref core)
{ alubox_fn_flags_nzc(core, alubox_eor); }

static void alubox_tsts(armvm_core_ref core)
{ alubox_fn_flags_nzc(core, alubox_and); }

/* **** */

static void alubox_cmns(armvm_core_ref core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	alubox_add(core);
	alubox_flags_add(core);
}

static void alubox_cmps(armvm_core_ref core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RD);

	alubox_sub(core);
	alubox_flags_sub(core);
}
