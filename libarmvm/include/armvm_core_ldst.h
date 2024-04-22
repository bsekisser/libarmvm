#pragma once

/* **** */

#include "armvm_core_exception.h"
#include "armvm_core_mem.h"

/* **** */

#include "armvm_coprocessor_cp15.h"

/* **** */

#include "libbse/include/bswap.h"
#include "libbse/include/shift_roll.h"

/* **** */

#include "local/core_reg.h"

/* **** */

static uint32_t __ldst__ea(armvm_core_p const core, const uint32_t sop, uint32_t* wb_ea)
{
	const unsigned bit_p = ARM_IR_LDST_BIT(P);

	assert(!(!bit_p && ARM_IR_LDST_BIT(W)));

	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	const uint32_t offset = ARM_IR_LDST_BIT(U) ? sop : -sop;

	const uint32_t pre_offset = bit_p ? offset : 0;
	const uint32_t post_offset = bit_p ? 0 : offset;

	const uint32_t ea = setup_rR_vR(core, ARMVM_TRACE_R(EA), ~0, rn + pre_offset);
	*wb_ea = ea + post_offset;

	return(ea);
}

static void __ldst__ea_wb(armvm_core_p const core, uint32_t wb_ea)
{
	if(!CCX) return;

	if(!ARM_IR_LDST_BIT(P) || ARM_IR_LDST_BIT(W))
		irGPR(N) = wb_ea;
}

/* **** */

static void __ldst_ldr(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	const unsigned ea_xx = ea & 3;

	if(ea_xx && CP15_REG1_BIT(A)) {
		armvm_core_exception_data_abort(core);
		return;
	}

	uint32_t rd = 0, *const p2rd = &rd;
	if(0 > armvm_core_mem_read(core, p2rd, ea, 4))
		return;

	__ldst__ea_wb(core, wb_ea);

	if(ea_xx && CP15_REG1_BIT(U))
		rd = _ror(rd, (ea_xx << 3));

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(T, rd & 1);
}

static void __ldst_ldrb(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	uint32_t rd = 0, *const p2rd = &rd;

	if(0 > armvm_core_mem_read(core, p2rd, ea, 1))
		return;

	__ldst__ea_wb(core, wb_ea);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), (uint32_t)(uint8_t)rd);
}

static void __ldst_ldrh(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	if((ea & 1) && CP15_REG1_BIT(A)) {
		armvm_core_exception_data_abort(core);
		return;
	}

	uint32_t rd = 0, *const p2rd = &rd;

	if(0 > armvm_core_mem_read(core, p2rd, ea, 2))
		return;

	if((ea & 1) && CP15_REG1_BIT(U))
		rd = bswap16(rd);

	__ldst__ea_wb(core, wb_ea);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), (uint32_t)(uint16_t)rd);
}

static void __ldst_ldrsb(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	uint32_t rd, *const p2rd = &rd;

	if(0 > armvm_core_mem_read(core, p2rd, ea, 1))
		return;

	__ldst__ea_wb(core, wb_ea);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D),
		(uint32_t)(int32_t)(int8_t)rd);
}

static void __ldst_ldrsh(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	if((ea & 1) && CP15_REG1_BIT(A)) {
		armvm_core_exception_data_abort(core);
		return;
	}

	uint32_t rd, *const p2rd = &rd;

	if(0 > armvm_core_mem_read(core, p2rd, ea, 2))
		return;

	if((ea & 1) && CP15_REG1_BIT(U))
		rd = bswap16(rd);

	__ldst__ea_wb(core, wb_ea);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D),
		(uint32_t)(int16_t)rd);
}

static void __ldst_str(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

//	if((ea & 3) && CP15_REG1_BIT(U))
//		return(armvm_core_exception_data_abort(core));

	const uint32_t rd = core_reg_src(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	if(0 > armvm_core_mem_write(core, ea, 4, rd))
		return;

	__ldst__ea_wb(core, wb_ea);
}

static void __ldst_strb(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	const uint32_t rd = core_reg_src(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	if(0 > armvm_core_mem_write(core, ea, 1, (uint32_t)(uint8_t)rd))
		return;

	__ldst__ea_wb(core, wb_ea);
}

static void __ldst_strh(armvm_core_p const core, const uint32_t sop)
{
	uint32_t wb_ea = 0, ea = __ldst__ea(core, sop, &wb_ea);
	if(!CCX) return;

	if((ea & 1) && CP15_REG1_BIT(U)) {
		armvm_core_exception_data_abort(core);
		return;
	}

	const uint32_t rd = core_reg_src(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	if(0 > armvm_core_mem_write(core, ea, 2, (uint32_t)(uint16_t)rd))
		return

	__ldst__ea_wb(core, wb_ea);
}
