#pragma once

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_shiftbox.h"
#include "libarm/include/arm_sop.h"

/* **** */

uint32_t _shifter_operand_immediate(armvm_core_p core)
{
	return(arm_shiftbox(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT, IF_CPSR(C)));
}

uint32_t _shifter_operand_c_immediate(armvm_core_p core)
{
	LOG();
	return(arm_shiftbox_c_immediate(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT));
}

uint32_t _shifter_operand_shift_immediate(armvm_core_p core)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), ARM_IR_DP_SHIFT_AMOUNT, IF_CPSR(C)));
}

uint32_t _shifter_operand_c_shift_immediate(armvm_core_p core)
{
	LOG();
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), ARM_IR_DP_SHIFT_AMOUNT));
}

uint32_t _shifter_operand_shift_register(armvm_core_p core)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), irGPR(S), IF_CPSR(C)));
}

uint32_t _shifter_operand_c_shift_register(armvm_core_p core)
{
	LOG();
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, irGPR(M), irGPR(S)));
}

uint32_t _shifter_operand(armvm_core_p core)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_immediate(core));

	if(ARM_IR_DP_I)
		return(_shifter_operand_shift_immediate(core));

	return(_shifter_operand_shift_register(core));
}

uint32_t _shifter_operand_c(armvm_core_p core)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_c_immediate(core));

	if(ARM_IR_DP_I)
		return(_shifter_operand_c_shift_immediate(core));

	return(_shifter_operand_c_shift_register(core));
}
