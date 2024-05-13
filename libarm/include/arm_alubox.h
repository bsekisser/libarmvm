#pragma once

/* **** */

#include "arm_dp.h"
#include "arm_gpr.h"

/* **** */

#include <stdint.h>

/* **** */

typedef struct arm_alubox_t* arm_alubox_p;
typedef struct arm_alubox_t {
	uint32_t gpr[_ARM_GPR_COUNT_];
//
	uint32_t cc;
	uint32_t cpsr;
}alubox_t;

uint32_t arm_alubox(arm_alubox_p alu, unsigned operation,
	const arm_gpr_t rd, const arm_gpr_t rn,
		const uint32_t sop_in);

uint32_t arm_alubox_shift_immediate(arm_alubox_p const alu,
	const unsigned alu_operation, const unsigned shift_type,
	const arm_gpr_t rm, const uint8_t rs);

uint32_t arm_alubox_shift_register(arm_alubox_p const alu,
	const unsigned alu_operation, const unsigned shift_type,
	const arm_gpr_t rm, const arm_gpr_t rs);
