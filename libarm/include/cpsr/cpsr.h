#pragma once

/* **** */

#define ARM_CPSR_BEXT(_v, _bit) (((_v) >> ARM_CPSR_BIT(_bit)) & 1)
#define ARM_CPSR_BIT(_) ARM_CPSR_BIT_##_
#define ARM_CPSR_M(_x) (ARM_CPSR_M_##_x)
#define ARM_CPSR_M32(_x) (ARM_CPSR_M(32) | ARM_CPSR_M(_x))

typedef enum arm_cpsr_bit_enum {
	ARM_CPSR_BIT_T = 5,
	ARM_CPSR_BIT_F = 6,
	ARM_CPSR_BIT_I = 7,
	ARM_CPSR_BIT_A = 8,
	ARM_CPSR_BIT_E = 9,
}arm_cpsr_bit_enum;

typedef enum arm_cpsr_mode_enum {
	ARM_CPSR_M_User,
	ARM_CPSR_M_FIQ,
	ARM_CPSR_M_IRQ,
	ARM_CPSR_M_Supervisor,
//
	ARM_CPSR_M_32 = 0x10,
//
	ARM_CPSR_M_Abort = 0x7,
	ARM_CPSR_M_Undefined = 0x8 | 3,
	ARM_CPSR_M_System = 0xf,
}arm_cpsr_mode_enum;

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

#include "libarm/include/apsr/apsr.h"

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t _arm_cpsr_read(arm_cpsr_tref cpsr);

void _arm_cpsr_write(arm_cpsr_ref cpsr, const uint32_t v);

__attribute__((warn_unused_result))
uint32_t arm_cpsr_read(arm_apsr_tref apsr, arm_cpsr_tref cpsr);

void arm_cpsr_write(arm_apsr_ref apsr, arm_cpsr_ref cpsr, const uint32_t v);
