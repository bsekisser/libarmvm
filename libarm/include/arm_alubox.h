#pragma once

/* **** */

#include "arm_dp.h"
#include "arm_gpr.h"

/* **** */

#include <stdint.h>

/* **** */

typedef struct arm_alubox_trace_t {
	uint32_t v;
	arm_reg_t r;
}arm_alubox_trace_t;

typedef struct arm_alubox_t* arm_alubox_p;
typedef struct arm_alubox_t {
	uint32_t gpr[_ARM_GPR_COUNT_];
//
	uint32_t cc;
	uint32_t cpsr;
}alubox_t;

int arm_alubox(arm_alubox_p alu, unsigned operation,
	const unsigned rd, const unsigned rn,
		const uint32_t sop_in)
{
	uint32_t sop = 0;
	switch(operation) {
		case ARM_BIC:
		case ARM_MVN:
			sop = ~sop_in;
			break;
		case ARM_RSB:
		case ARM_RSC:
			sop = alu->gpr[rn];
			break;
		default:
			sop = sop_in;
			break;
	}

	uint32_t rn = 0;
	switch(operation) {
		case ARM_MOV: break;
		case ARM_MVN: break;
		case ARM_RSB:
		case ARM_RSC:
			rn = sop_in;
			break;
		default:
			rn = alu->gpr[rn];
			break;
	}

	uint32_t rd = rn;

	switch(operation) {
		case ARM_ADC:
		case ARM_ADD:
		case ARM_CMN:
			rd += sop;
			break;
		case ARM_AND:
		case ARM_BIC:
		case ARM_TST:
			rd &= sop;
			break;
		case ARM_CMP:
		case ARM_NEG:
		case ARM_RSB:
		case ARM_RSC:
		case ARM_SBC:
		case ARM_SUB:
			rd -= sop;
			break;
		case ARM_EOR:
		case ARM_TEQ:
			rd ^= sop;
			break;
		case ARM_MOV:
		case ARM_MVN:
			rd = sop;
			break;
		case ARM_MUL:
			rd *= sop;
			break;
		case ARM_ORR:
			rd |= sop;
			break;
		default:
			return(-1);
	}

	const unsigned carry_in = ARM_CPSRx_BEXT(alu->cpsr, C);

	switch(operation) {
		case ARM_ADC:
			rd += !!carry_in;
			break;
		case ARM_RSC:
		case ARM_SBC:
			rd -= !!(!carry_in);
			break;
	}

	alu->rd = rd;
	if(0 > alu->cc)
		return(0);

	switch(operation) {
		case ARM_CMN:
		case ARM_CMP:
		case ARM_TEQ:
		case ARM_TST:
			break;
		default:
			alu->gpr[rd] = rd;
			break;;
	}

	if(s && (ARM_GPR_PC != rd)) {
		switch(operation) {
			case ARM_ADC:
			case ARM_ADD:
			case ARM_CMN:
			case ARM_NEG:
				arm_alubox_flags_x_add_sub(alu, rd, rn, sop);
				break;
			case ARM_CMP:
			case ARM_RSB:
			case ARM_RSC:
			case ARM_SBC:
			case ARM_SUB:
				arm_alubox_flags_x_add_sub(alu, rd, rn, ~sop);
				break;
			default:
				arm_alubox_flags_nz(alu, rd);
				break;
		}
	}
}

int arm_alubox_shift_register(arm_alubox_p const alu, const unsigned alu_operation,
	const unsigned rd, const unsigned rn,
	const unsigned shift_type, const uint32_t rm, const uint8_t rs)
{
	if(rs)
}

int arm_alubox_shift_register(arm_alubox_p const alu, const unsigned alu_operation,
	const unsigned rd, const unsigned rn,
	const unsigned shift_type, const unsigned rm, const unsigned rs)
