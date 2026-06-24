#pragma once

/* **** */

#define ARM_CPSR_BIT(_) ARM_CPSR_BIT_##_

typedef enum arm_cpsr_bit_enum {
	ARM_CPSR_BIT_T = 5,
	ARM_CPSR_BIT_F = 6,
	ARM_CPSR_BIT_I = 7,
	ARM_CPSR_BIT_A = 8,
	ARM_CPSR_BIT_E = 9,
}arm_cpsr_bit_enum;

typedef union arm_cpsr_tag* arm_cpsr_ptr;
typedef arm_cpsr_ptr const arm_cpsr_ref;

typedef union arm_cpsr_tag {
	unsigned raw_flags;
	struct {
		struct {
			char abort:1;
			char endian:1;
			char fiq:1;
			char irq:1;
			char thumb:1;
		};
		char mode:5;
	};
}arm_cpsr_t;
typedef arm_cpsr_t const arm_cpsr_tref;

/* **** */

#include "apsr/apsr.h"

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t _arm_cpsr_read(arm_cpsr_tref cpsr);

void _arm_cpsr_write(arm_cpsr_ref cpsr, const uint32_t v);

__attribute__((warn_unused_result))
uint32_t arm_cpsr_read(arm_apsr_tref apsr, arm_cpsr_tref cpsr);

void arm_cpsr_write(arm_apsr_ref apsr, arm_cpsr_ref cpsr, const uint32_t v);
