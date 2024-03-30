#pragma once

/* **** */

#include "arm_coprocessor_cp15.h"
#include "arm_exception.h"
#include "arm_mem.h"

/* **** */

#include "shift_roll.h"

/* **** */

static uint32_t __ldst__ea(uint32_t sop, uint32_t* wb_ea, armvm_p avm)
{
	const unsigned bit_p = ARM_IR_LDST_BIT(P);

	assert(!(!bit_p && ARM_IR_LDST_BIT(W)));

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));
	const uint32_t offset = ARM_IR_LDST_BIT(U) ? sop : -sop;

	const uint32_t pre_offset = bit_p ? offset : 0;
	const uint32_t post_offset = bit_p ? 0 : offset;

	const uint32_t ea = setup_rR_vR(avm, ARM_TRACE_R(EA), ~0, rn + pre_offset);
	*wb_ea = ea + post_offset;

	return(ea);
}

static void __ldst__ea_wb(uint32_t wb_ea, armvm_p avm)
{
	if(!CCX) return;

	if(!ARM_IR_LDST_BIT(P) || ARM_IR_LDST_BIT(W))
		rGPR(N) = wb_ea;
}

/* **** */

static void __ldst_ldr(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	const unsigned ea_xx = ea & 3;

	uint32_t rd = 0, *const p2rd = &rd;
	if(0 > arm_mem_read(p2rd, ea, 4, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);

	if(ea_xx && CP15_REG1_BIT(U))
		rd = _ror(rd, (ea_xx << 3));

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);

	if(rR_IS_PC(D) && (arm_v5t <= vmCONFIG->version))
		ARM_CPSR_BMAS(T, rd & 1);
}

static void __ldst_ldrb(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	uint32_t rd = 0, *const p2rd = &rd;

	if(0 > arm_mem_read(p2rd, ea, 1, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), (uint32_t)(uint8_t)rd);
}

static void __ldst_ldrh(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	if((ea & 1) && !CP15_REG1_BIT(U))
		return(arm_exception(ARM_VM_EXCEPTION_DataAbort, avm));

	uint32_t rd = 0, *const p2rd = &rd;

	if(0 > arm_mem_read(p2rd, ea, 2, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), (uint32_t)(uint16_t)rd);
}

static void __ldst_ldrsb(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	uint32_t rd, *const p2rd = &rd;

	if(0 > arm_mem_read(p2rd, ea, 1, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D),
		(uint32_t)(int32_t)(int8_t)rd);
}

static void __ldst_ldrsh(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	if((ea & 1) && !CP15_REG1_BIT(U))
		return(arm_exception(ARM_VM_EXCEPTION_DataAbort, avm));

	uint32_t rd, *const p2rd = &rd;

	if(0 > arm_mem_read(p2rd, ea, 2, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D),
		(uint32_t)(int16_t)rd);
}

static void __ldst_str(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

//	if((ea & 3) && CP15_REG1_BIT(U))
//		soc_core_exception(core, _EXCEPTION_DataAbort);

	const uint32_t rd = arm_reg_src(avm, ARM_TRACE_R(D), ARM_IR_R(D));

	if(0 > arm_mem_write(ea, 4, rd, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);
}

static void __ldst_strb(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	const uint32_t rd = arm_reg_src(avm, ARM_TRACE_R(D), ARM_IR_R(D));

	if(0 > arm_mem_write(ea, 1, (uint32_t)(uint8_t)rd, avm))
		return;

	__ldst__ea_wb(wb_ea, avm);
}

static void __ldst_strh(uint32_t sop, armvm_p avm)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(sop, &wb_ea, avm);
	if(!CCX) return;

	if((ea & 1) && !CP15_REG1_BIT(U))
		return(arm_exception(ARM_VM_EXCEPTION_DataAbort, avm));

	const uint32_t rd = arm_reg_src(avm, ARM_TRACE_R(D), ARM_IR_R(D));

	if(0 > arm_mem_write(ea, 2, (uint32_t)(uint16_t)rd, avm))
		return

	__ldst__ea_wb(wb_ea, avm);
}
