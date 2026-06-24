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

__attribute__((visibility("default")))
uint32_t arm_apsr_shiftbox(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs)
{
	uint32_t result = rm;

	if(rs) {
		unsigned carry_out = 0;

		switch(shift_type) {
			case ARM_SOP_ASR: result = asr32_vc(rm, rs, &carry_out); break;
			case ARM_SOP_LSR: result = lsr32_vc(rm, rs, &carry_out); break;
			case ARM_SOP_LSL: result = lsl32_vc(rm, rs, &carry_out); break;
			case ARM_SOP_ROR: result = ror32_vc(rm, rs, &carry_out); break;
			case ARM_SOP_RRX: result = _RRX(rm, APSRp(c), carry_out); break;
			default:
				LOGu(shift_type);
				LOG_ACTION(exit(-1));
		}

		APSRp_OUT(c) = carry_out;
	}

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
			return(arm_apsr_shiftbox(apsr, ARM_SOP_RRX, rm, 0));
		default: break;
	}

	return(arm_apsr_shiftbox(apsr, shift_type, rm, rs));
}
