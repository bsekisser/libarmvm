#include "arm_alubox.h"

/* **** */

#include "arm_dp.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/shift_roll_32.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

uint32_t arm_alubox(arm_dp_opcode_tref operation, uint32_t *const rd, const uint32_t rn,
	const uint32_t sop, const unsigned carry_in)
{
	uint32_t result = 0;

	switch(operation) {
		case ARM_ADC:
			result = rn + sop + carry_in;
			break;
		case ARM_ADD: case ARM_CMN:
			result = rn + sop;
			break;
		case ARM_AND: case ARM_TST:
			result = rn & sop;
			break;
		case ARM_ASR:
			_LOGx32(result = asr32(rn, sop));
			break;
		case ARM_BIC:
			result = rn & ~sop;
			break;
		case ARM_CMP: case ARM_SUB:
			result = rn - sop;
			break;
		case ARM_EOR: case ARM_TEQ:
			result = rn ^ sop;
			break;
		case ARM_LSL:
			_LOGx32(result = lsl32(rn, sop));
			break;
		case ARM_LSR:
			_LOGx32(result = lsr32(rn, sop));
			break;
		case ARM_MOV:
			result = sop;
			break;
		case ARM_MUL:
			result = rn * sop;
			break;
		case ARM_MVN:
			result = ~sop;
			break;
		case ARM_NEG:
			result = -sop;
			break;
		case ARM_ORR:
			result = rn | sop;
			break;
		case ARM_ROR:
			_LOGx32(result = ror32(rn, sop));
			break;
		case ARM_RSC:
			result = sop - rn - carry_in;
			break;
		case ARM_RSB:
			result = sop - rn;
			break;
		case ARM_SBC:
			result = rn - sop - carry_in;
			break;
	}

	if(rd)
		*rd = result;

	return(result);
}
