#include "arm_alubox.h"

/* **** */

#include "arm_dp.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

uint32_t arm_alubox(unsigned operation, uint32_t *const rd, uint32_t rn,
	uint32_t sop, unsigned carry_in)
{
	uint32_t result = 0;

	switch(operation) {
	case ARM_BIC:
	case ARM_MVN:
		sop = ~sop;
	break;
	case ARM_MOV:
	case ARM_RSB:
	case ARM_RSC:
		result = sop;
	default:
		result = rn;
	break;
	}

	switch(operation) {
	case ARM_ADC:
	case ARM_ADD:
	case ARM_CMN:
		result += sop;
	break;
	case ARM_AND:
	case ARM_BIC:
	case ARM_TST:
		result &= sop;
	break;
	case ARM_CMP:
	case ARM_SBC:
	case ARM_SUB:
		result -= sop;
	break;
	case ARM_EOR:
	case ARM_TEQ:
		result ^= sop;
	break;
//	case ARM_MOV:
	case ARM_MVN:
		result = sop;
	break;
	case ARM_ORR:
		result |= sop;
	break;
	case ARM_RSC:
	case ARM_RSB:
		result -= rn;
	break;
	default:
		exit(-1);
	}

	switch(operation) {
	case ARM_ADC:
		result += carry_in;
	break;
	case ARM_RSC:
	case ARM_SBC:
		result -= carry_in;
	break;
	}

	if(rd)
		*rd = result;

	return(result);
}
