#pragma once

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_shiftbox.h"
#include "libarm/include/arm_sop.h"

/* **** */

#include "libbse/include/unused.h"

/* **** */

UNUSED_FN
static uint32_t _shifter_operand_immediate(armvm_core_p const core)
{
	return(arm_shiftbox(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT, IF_CPSR(C)));
}

UNUSED_FN
static uint32_t _shifter_operand_c_immediate(armvm_core_p const core)
{
	return(arm_shiftbox_c_immediate(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT));
}

UNUSED_FN
static uint32_t _shifter_operand_shift_immediate(armvm_core_p const core)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), ARM_IR_DP_SHIFT_AMOUNT, IF_CPSR(C)));
}

UNUSED_FN
static uint32_t _shifter_operand_c_shift_immediate(armvm_core_p const core)
{
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), ARM_IR_DP_SHIFT_AMOUNT));
}

UNUSED_FN
static uint32_t _shifter_operand_shift_register(armvm_core_p const core)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), irGPR(S), IF_CPSR(C)));
}

UNUSED_FN
static uint32_t _shifter_operand_c_shift_register(armvm_core_p const core)
{
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), irGPR(S)));
}

UNUSED_FN
static uint32_t _shifter_operand(armvm_core_p const core)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_immediate(core));

	if(ARM_IR_DP_I)
		return(_shifter_operand_shift_register(core));

	return(_shifter_operand_shift_immediate(core));
}

UNUSED_FN
static uint32_t _shifter_operand_c(armvm_core_p const core)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_c_immediate(core));

	if(ARM_IR_DP_I)
		return(_shifter_operand_c_shift_register(core));

	return(_shifter_operand_c_shift_immediate(core));
}
