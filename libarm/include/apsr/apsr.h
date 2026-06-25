#pragma once

/* **** */

#include "libbse/include/static_assert.h"

/* **** */

#define _ARM_APSR_BSET(_bit) _ARM_APSR_BSET_AS(_bit, 1U)
#define _ARM_APSR_BSET_AS(_bit, _as) ((_as) << ARM_APSR_BIT(_bit))
#define ARM_APSR_BEXT(_v, _bit) (((_v) >> ARM_APSR_BIT(_bit)) & 1U)
#define ARM_APSR_BIT(_) (ARM_APSR_BIT_##_)
#define ARM_APSR_BSET(_v, _bit) ((_v) | _ARM_APSR_BSET(_bit))

#define ARM_APSR_MASK_NZ (_ARM_APSR_BSET(N) | _ARM_APSR_BSET(Z))
#define ARM_APSR_MASK_NZC (_ARM_APSR_BSET(C) | ARM_APSR_MASK_NZ)
#define ARM_APSR_MASK_NZCV (_ARM_APSR_BSET(V) | ARM_APSR_MASK_NZC)

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

STATIC_ASSERT(1 == sizeof(_arm_apsr_t));

typedef union arm_apsr_tag* arm_apsr_ptr;
typedef arm_apsr_ptr const arm_apsr_ref;

typedef union arm_apsr_tag {
	unsigned raw_flags;
	struct {
		// current flags going in to functions
		union {
			_arm_apsr_tref in;
			_arm_apsr_t set;
		};
		// pending flags being returned
//		_arm_apsr_t out;
		struct {
			char sco:1; // shift_carry_out
			char set:1;
			char sticky:1;
		}flags;
	};
}arm_apsr_t;
typedef arm_apsr_t const arm_apsr_tref;

STATIC_ASSERT(4 == sizeof(arm_apsr_t));

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_apsr_read(arm_apsr_tref apsr);

__attribute__((warn_unused_result))
uint32_t arm_apsr_read_masked(arm_apsr_tref apsr, const uint32_t mask);

void arm_apsr_write(arm_apsr_ref apsr, const uint32_t v);
