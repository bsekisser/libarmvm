#pragma once

/* **** */

#include "armvm_core.h"

#include "local/alubox.h"

/* **** */

#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_shiftbox.h"

#include "libbse/include/unused.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t alubox_shift(armvm_core_p const core, const unsigned operation,
	const unsigned s, const unsigned shift_type, const unsigned immediate)
{
	if(immediate)
		vR(SOP) = arm_shiftbox_immediate(shift_type, vR(M), vR(S), IF_CPSR(C));
	else
		vR(SOP) = arm_shiftbox(shift_type, vR(M), vR(S), IF_CPSR(C));

	switch(operation) {
		case ARM_ADC:
		case ARM_ADD:
		case ARM_CMN:
		case ARM_CMP:
		case ARM_RSB:
		case ARM_RSC:
		case ARM_SBC:
		case ARM_SUB:
			break;
		default:
			if(immediate)
				ARM_CPSR_BMAS(C, arm_shiftbox_c_immediate(shift_type, vR(M), vR(S)));
			else
				ARM_CPSR_BMAS(C, arm_shiftbox_c(shift_type, vR(M), vR(S)));
			break;
	}

	return(alubox(core, operation, s, 0));
}

UNUSED_FN
static uint32_t alubox_shift_immediate(armvm_core_p const core, const unsigned operation,
	const unsigned s, const unsigned shift_type)
{ return(alubox_shift(core, operation, s, shift_type, 1)); }

UNUSED_FN
static uint32_t alubox_shift_register(armvm_core_p const core, const unsigned operation,
	const unsigned s, const unsigned shift_type)
{ return(alubox_shift(core, operation, s, shift_type, 0)); }
