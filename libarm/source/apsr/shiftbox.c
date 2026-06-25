#include "apsr/shiftbox.h"

/* **** */

#include "apsr/apsr.h"
#include "apsr/glue.h"

/* **** */

#include "sop.h" // TODO: conversion to shift_type

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/shift_roll_32.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

static inline
uint32_t _arm_shiftbox(arm_sop_eref shift_type, const uint32_t rm, const unsigned rs, const unsigned carry_in, unsigned *const carry_out)
{
	*carry_out = carry_in;

	switch(shift_type) {
		case ARM_SOP_ASR:
			if(31 < rs) {
				*carry_out = (0 > (int32_t)rm);
				return((0 > (int32_t)rm) ? ~0 : 0);
			} else
				return(asr32_vc(rm, rs, carry_out));
		case ARM_SOP_LSL:
			if(31 < rs) {
				*carry_out = ((32 == rs) ? (rm & 1) : 0);
				return(0);
			} else
				return(lsl32_vc(rm, rs, carry_out));
		case ARM_SOP_LSR:
			if(31 < rs) {
				*carry_out = ((32 == rs) ? (0 > (int32_t)rm) : 0);
				return(0);
			} else
				return(lsr32_vc(rm, rs, carry_out));
		case ARM_SOP_ROR:
			return(ror32_vc(rm, rs, carry_out));
		case ARM_SOP_RRX:
			return(_RRX(rm, carry_in, *carry_out));
		default:
			LOGu(shift_type);
			LOG_ACTION(exit(-1));
	}

	return(rm);
}

__attribute__((visibility("default")))
uint32_t arm_apsr_shiftbox(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs)
{
	unsigned carry_out = 0;
	const uint32_t result = _arm_shiftbox(shift_type, rm, rs, APSRp(c), &carry_out);

	APSRp_FLAGS(sco) = carry_out;

	return(result);
}

__attribute__((visibility("default")))
uint32_t arm_apsr_shiftbox_immediate(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs)
{
	if(!rs) switch(shift_type) {
		case ARM_SOP_ASR:
		case ARM_SOP_LSR:
			return(arm_apsr_shiftbox(apsr, shift_type, rm, 32));
		case ARM_SOP_ROR:
			return(arm_apsr_shiftbox(apsr, ARM_SOP_RRX, rm, rs));
		default: break;
	}

	return(arm_apsr_shiftbox(apsr, shift_type, rm, rs));
}
