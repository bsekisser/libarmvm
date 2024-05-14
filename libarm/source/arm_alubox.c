#include "arm_alubox.h"

/* **** */

#include "arm_cpsr.h"
#include "arm_dp.h"
#include "arm_gpr.h"
#include "arm_sop.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

uint32_t arm_alubox(arm_alubox_p alu,
	const unsigned alu_operation, const unsigned s,
	const arm_gpr_t rrd, const arm_gpr_t rrn,
		const uint32_t sop_in)
{
	uint32_t sop = 0;
	switch(alu_operation) {
		case ARM_BIC:
		case ARM_MVN:
			sop = ~sop_in;
			break;
		case ARM_RSB:
		case ARM_RSC:
			sop = alu->gpr[rrn];
			break;
		default:
			sop = sop_in;
			break;
	}

	uint32_t rn = 0;
	switch(alu_operation) {
		case ARM_MOV: break;
		case ARM_MVN: break;
		case ARM_RSB:
		case ARM_RSC:
			rn = sop_in;
			break;
		default:
			rn = alu->gpr[rrn];
			break;
	}

	uint32_t rd = rn;

	switch(alu_operation) {
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
			LOG("operation: 0x%08x", alu_operation);
			LOG_ACTION(abort());
	}

	const unsigned carry_in = ARM_CPSRx_BEXT(alu->cpsr, C);

	switch(alu_operation) {
		case ARM_ADC:
			rd += !!carry_in;
			break;
		case ARM_RSC:
		case ARM_SBC:
			rd -= !!(!carry_in);
			break;
	}

	if(!alu->cc)
		return(rd);

	switch(alu_operation) {
		case ARM_CMN:
		case ARM_CMP:
		case ARM_TEQ:
		case ARM_TST:
			break;
		default:
			alu->gpr[rrd] = rd;
			break;;
	}

	if(s && (ARM_GPR_R(PC) != rrd)) {
		switch(alu_operation) {
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

	return(rd);
}

uint32_t arm_alubox_shift_immediate(arm_alubox_p const alu,
	const unsigned alu_operation, const unsigned s,
	const unsigned shift_type, const arm_gpr_t rrm, const uint8_t rs)
{
	const uint32_t sop = arm_shiftbox_immediate(alu, shift_type, rrm, rs);

	switch(alu_operation) {
		case ARM_AND:
		case ARM_BIC:
		case ARM_EOR:
		case ARM_MOV:
		case ARM_MUL:
		case ARM_MVN:
		case ARM_TEQ:
		case ARM_TST:
			if(s) arm_shiftbox_c_immediate(alu, shift_type, rrm, rs);
			break;
		case ARM_ADC:
		case ARM_ADD:
		case ARM_CMN:
		case ARM_CMP:
		case ARM_RSB:
		case ARM_SUB:
			break;
		default:
			LOG_ACTION(abort());
			break;
	}

	return(sop);
}

uint32_t arm_alubox_shift_register(arm_alubox_p const alu,
	const unsigned alu_operation, const unsigned s,
	const unsigned shift_type, const arm_gpr_t rrm, const arm_gpr_t rrs)
{
	const uint32_t sop = arm_shiftbox(alu, shift_type, rrm, rrs);

	switch(alu_operation) {
		case ARM_AND:
		case ARM_BIC:
		case ARM_EOR:
		case ARM_MOV:
		case ARM_MUL:
		case ARM_MVN:
		case ARM_TEQ:
		case ARM_TST:
			if(s) arm_shiftbox_c(alu, shift_type, rrm, rrs);
			break;
		case ARM_ADC:
		case ARM_ADD:
		case ARM_CMN:
		case ARM_CMP:
		case ARM_RSB:
		case ARM_SUB:
			break;
		default:
			LOG_ACTION(abort());
			break;
	}

	return(sop);
}
