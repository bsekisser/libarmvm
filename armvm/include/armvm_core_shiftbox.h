#pragma once

/* **** */

#include "arm_cpsr.h"
#include "arm_ir.h"
#include "arm_shiftbox.h"
#include "arm_sop.h"

/* **** */

uint32_t _shifter_operand_immediate(armvm_p avm)
{
	return(arm_shiftbox(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT, IF_CPSR(C)));
}

uint32_t _shifter_operand_c_immediate(armvm_p avm)
{
	return(arm_shiftbox_c_immediate(ARM_SOP_ROR, ARM_IR_DPI_IMMEDIATE, ARM_IR_DPI_ROTATE_AMOUNT));
}

uint32_t _shifter_operand_shift_immediate(armvm_p avm)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, rGPR(M), ARM_IR_DP_SHIFT_AMOUNT, IF_CPSR(C)));
}

uint32_t _shifter_operand_c_shift_immediate(armvm_p avm)
{
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, rGPR(M), ARM_IR_DP_SHIFT_AMOUNT));
}

uint32_t _shifter_operand_shift_register(armvm_p avm)
{
	return(arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, rGPR(M), rGPR(S), IF_CPSR(C)));
}

uint32_t _shifter_operand_c_shift_register(armvm_p avm)
{
	return(arm_shiftbox_c_immediate(ARM_IR_DP_SHIFT_TYPE, rGPR(M), rGPR(S)));
}

uint32_t _shifter_operand(armvm_p avm)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_immediate(avm));

	if(ARM_IR_DP_I)
		return(_shifter_operand_shift_immediate(avm));

	return(_shifter_operand_shift_register(avm));
}

uint32_t _shifter_operand_c(armvm_p avm)
{
	if(ARM_IR_DPI)
		return(_shifter_operand_c_immediate(avm));

	if(ARM_IR_DP_I)
		return(_shifter_operand_c_shift_immediate(avm));

	return(_shifter_operand_c_shift_register(avm));
}
