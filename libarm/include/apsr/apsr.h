#pragma once

/* **** */

#define ARM_APSR_BIT(_) ARM_APSR_BIT_##_

typedef enum arm_apsr_bit_enum {
	ARM_APSR_BIT_V = 28,
	ARM_APSR_BIT_C = 29,
	ARM_APSR_BIT_Z = 30,
	ARM_APSR_BIT_N = 31,
}arm_apsr_bit_enum;

typedef union _arm_apsr_tag {
	char raw_flags;
	struct {
		char c:1;
		char n:1;
		char v:1;
		char z:1;
	};
}_arm_apsr_t;
typedef _arm_apsr_t const _arm_apsr_tref;

_Static_assert(1 == sizeof(_arm_apsr_t));

typedef union arm_apsr_tag* arm_apsr_ptr;
typedef arm_apsr_ptr const arm_apsr_ref;

typedef union arm_apsr_tag {
	unsigned raw_flags;
	struct {
		// current flags going in to functions
		_arm_apsr_tref in;
		// pending flags being returned
		_arm_apsr_t out;
	};
}arm_apsr_t;
typedef arm_apsr_t const arm_apsr_tref;

_Static_assert(4 == sizeof(arm_apsr_t));

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_apsr_read(arm_apsr_tref apsr);

void arm_apsr_write(arm_apsr_ref apsr, const uint32_t v);
