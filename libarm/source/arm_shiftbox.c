#include "arm_shiftbox.h"

/* **** */

#include "arm_sop.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/log.h"
#include "libbse/include/shift_roll.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

uint32_t arm_shiftbox(unsigned shift_type, uint32_t rm, uint32_t rs, unsigned carry_in)
{
	switch(shift_type) {
	case ARM_SOP_ASR:
		return(_asr(rm, rs));
	break;
	case ARM_SOP_LSL:
		return(_lsl(rm, rs));
	break;
	case ARM_SOP_LSR:
		return(_lsr(rm, rs));
	break;
	case ARM_SOP_ROR:
		return(_ror(rm, rs));
	break;
	case ARM_SOP_RRX:
		return(_rrx_v(rm, carry_in));
	break;
	default:
		LOG_ACTION(exit(-1));
	}

	return(0xdeadbeef);
}

uint32_t arm_shiftbox_c(unsigned shift_type, uint32_t rm, uint32_t rs)
{
	switch(shift_type) {
	case ARM_SOP_ASR:
		return(_asr_c(rm, rs));
	break;
	case ARM_SOP_LSL:
		return(_lsl_c(rm, rs));
	break;
	case ARM_SOP_LSR:
		return(_lsr_c(rm, rs));
	break;
	case ARM_SOP_ROR:
		return(_ror_c(rm, rs));
	break;
	case ARM_SOP_RRX:
		return(_rrx_c(rm));
	break;
	default:
		LOG_ACTION(exit(-1));
	}

	return(0xdeadbeef);
}

uint32_t arm_shiftbox_immediate(unsigned shift_type, uint32_t rm, uint32_t rs, unsigned carry_in)
{
	if(!rs) switch(shift_type) {
	case ARM_SOP_ASR:
		return(((int32_t)rm < 0) ? ~0 : 0);
	case ARM_SOP_LSR:
		return(0);
	break;
	case ARM_SOP_ROR:
		return(_rrx_v(rm, carry_in));
	break;
	}

	return(arm_shiftbox(shift_type, rm, rs, carry_in));
}

uint32_t arm_shiftbox_c_immediate(unsigned shift_type, uint32_t rm, uint32_t rs)
{
	if(!rs) switch(shift_type) {
	case ARM_SOP_ASR:
	case ARM_SOP_LSR:
		return(BEXT(rm, 31));
	break;
	case ARM_SOP_ROR:
		return(_rrx_c(rm));
	break;
	}

	return(arm_shiftbox_c(shift_type, rm, rs));
}
