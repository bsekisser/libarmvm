#pragma once

/* **** */

#include "armvm_core.h"

#include "local/alubox.h"
#include "local/core_reg.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libarm/include/arm_shiftbox.h"
#include "libarm/include/arm_sop.h"

/* **** */

#include <assert.h>
#include <stdint.h>

/* **** */

static uint32_t alubox_thumb(armvm_core_p const core, const unsigned operation,
	const unsigned s)
{
	unsigned shift_type = ~0U;
	switch(operation) {
		case ARM_ASR:
			shift_type = ARM_SOP_ASR;
			break;
		case ARM_LSL:
			shift_type = ARM_SOP_LSL;
			break;
		case ARM_LSR:
			shift_type = ARM_SOP_LSR;
			break;
		case ARM_ROR:
			shift_type = ARM_SOP_ROR;
			break;
		default:
			setup_vR(core, ARMVM_TRACE_R(SOP), vR(M));
			return(alubox(core, operation, s, 0));
	}

	assert(~0U != shift_type);

	core_reg_src_load(core, ARMVM_TRACE_R(N));
	const uint32_t valid_rs = vR(M) & 0xff;

	vR(SOP) = arm_shiftbox(shift_type, vR(N), valid_rs, IF_CPSR(C));
	ARM_CPSR_BMAS(C, !!arm_shiftbox_c(shift_type, vR(N), valid_rs));
	return(alubox(core, ARM_MOV, s, 0));
}
