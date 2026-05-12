#include "arm_shiftbox.h"

/* **** */

#include "arm_sop.h"

/* **** */

#include "libbse/include/bitfield.h"
//#include "libbse/include/bitops32.h"
#include "libbse/include/log.h"
#include "libbse/include/shift_roll_32.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

uint32_t arm_shiftbox(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in)
{
	switch(shift_type) {
	case ARM_SOP_ASR:
		return(asr32(rm, rs));
	break;
	case ARM_SOP_LSL:
		return(lsl32(rm, rs));
	break;
	case ARM_SOP_LSR:
		return(lsr32(rm, rs));
	break;
	case ARM_SOP_ROR:
		return(ror32(rm, rs));
	break;
	case ARM_SOP_RRX:
		return(rrx32_v(rm, carry_in));
	break;
	case _ARM_SOP_COUNT_:
	default:
		LOG_ACTION(exit(-1));
	}

	return(0xdeadbeef);
}

uint32_t arm_shiftbox_c(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs)
{
	switch(shift_type) {
	case ARM_SOP_ASR:
		return(asr32_c(rm, rs));
	break;
	case ARM_SOP_LSL:
		return(lsl32_c(rm, rs));
	break;
	case ARM_SOP_LSR:
		return(lsr32_c(rm, rs));
	break;
	case ARM_SOP_ROR:
		return(ror32_c(rm, rs));
	break;
	case ARM_SOP_RRX:
		return(rrx32_c(rm));
	break;
	case _ARM_SOP_COUNT_:
	default:
		LOG_ACTION(exit(-1));
	}

	return(0xdeadbeef);
}

uint32_t arm_shiftbox_immediate(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in)
{
	if(!rs) switch(shift_type) {
	case ARM_SOP_ASR:
		return((0 > (int32_t)rm) ? ~0 : 0);
	case ARM_SOP_LSR:
		return(0);
	break;
	case ARM_SOP_ROR:
		return(rrx32_v(rm, carry_in));
	break;
//
	case ARM_SOP_LSL:
	case ARM_SOP_RRX:
		break;
	case _ARM_SOP_COUNT_:
	default:
		LOG_ACTION(exit(-1));
	}

	return(arm_shiftbox(shift_type, rm, rs, carry_in));
}

uint32_t arm_shiftbox_c_immediate(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs)
{
	if(!rs) switch(shift_type) {
	case ARM_SOP_ASR:
	case ARM_SOP_LSR:
		return((0 > (int32_t)rm) ? 1 : 0);
	break;
	case ARM_SOP_ROR:
		return(rrx32_c(rm));
	break;
	case ARM_SOP_LSL:
	case ARM_SOP_RRX:
		break;
	case _ARM_SOP_COUNT_:
	default:
		LOG_ACTION(exit(-1));
	}

	return(arm_shiftbox_c(shift_type, rm, rs));
}
