#pragma once

/* **** */

#include "alubox.h"
#include "shiftbox.h"

/* **** */

typedef struct data_processing_tag {
	arm_condition_enum cc;
	arm_dp_opcode_enum opcode;
//
	unsigned char rd:4;
	unsigned char rn:4;
	unsigned char i:1;
	unsigned char s:1;
//
	shift_operand_t sop;
}data_processing_t;


static
uint32_t data_processing(data_processing_tref dp, armvm_ref avm)
{
	const unsigned is_mov = (015 == (~2 & dp.opcode));

	shift_operand_result_tref sop = shift_operand_dp(dp.sop, avm, dp.s);

	switch(dp.opcode) {
		case ARM_ADC:
			dp_flags_adc(dp, avm, rn, sop);
			break;
		case ARM_ADD: case ARM_CMN:
			dp_flags_add(dp, avm, rn, sop);
			break;
		case ARM_AND: case ARM_TST:
			dp_flags_nzc(dp, avm, rn & sop.v);
			break;
		case ARM_BIC:
			dp_flags_nzc(dp, avm, rn & ~sop.v);
			break;
		case ARM_MOV:
			dp_flags_nzc(dp, avm, sop.v);
			break;
		case ARM_MVN:
			dp_flags_nzc(dp, avm, ~sop.v);
			break;
