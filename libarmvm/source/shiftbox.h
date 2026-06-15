#pragma once

/* **** */

#include "armvm.h"

/* **** */

#include "libarm/include/sop.h"

/* **** */

#include <stdint.h>

/* **** */

uint32_t shiftbox(armvm_ref avm, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs, unsigned *const carry_out)
{
	const unsigned carry_in = ARM_CPSR_BEXT(C);

	switch(shift_type) {
		case ARM_SOP_ASR: return(asr32_vc(rm, rs, carry_out));
		case ARM_SOP_LSL: return(lsl32_vc(rm, rs, carry_out));
		case ARM_SOP_LSR: return(lsr32_vc(rm, rs, carry_out));
		case ARM_SOP_ROR: return(ror32_vc(rm, rs, carry_out));
		case ARM_SOP_RRx: return(rrx32_vc(rm, carry_in, carry_out);
	}
	return(rm);
}

uint32_t shiftbox_immediate(armvm_ref avm, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs, unsigned *const carry_out)
{
	if(!rs) {
		switch(shift_type) {
			case ARM_SOP_ASR:
			case ARM_SOP_LSR:
				return(shiftbox(avm, shift_type, rm, 32, carry_out));
			case ARM_SOP_ROR:
				return(shiftbox(avm, ARM_SOP_RRX, rm, 0, carry_out));
		}
	}

	return(shiftbox(avm, shift_type, rm, rs, carry_out));
}
