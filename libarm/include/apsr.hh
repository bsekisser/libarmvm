#pragma once

/* **** */

#include "libbse/include/shift_roll_32.h"

#include <stdint.h>

/* **** */

static inline __attribute__((always_inline, warn_unused_result))
unsigned __builtin_add_overflow_vf(const int32_t s1, const int32_t s2, const uint32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_add_overflow(s1, s2, &result)
		| __builtin_add_overflow(result, carry_in, &result));
}

static inline __attribute__((always_inline, warn_unused_result))
unsigned __builtin_sub_overflow_vf(const int32_t s1, const int32_t s2, const uint32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_sub_overflow(s1, s2, &result)
		| __builtin_sub_overflow(result, carry_in, &result));
}

typedef union arm_apsr_tag* arm_apsr_ptr;
typedef union arm_apsr_tag& arm_apsr_ref;

typedef union arm_apsr_tag {
	__attribute__((always_inline, warn_unused_result))
	uint32_t _adds(const uint32_t s1, const uint32_t s2, const uint32_t carry_in)
	{
		uint32_t result = 0;

		flags.c
			= __builtin_add_overflow(s1, s2, &result)
			| __builtin_add_overflow(result, carry_in, &result);

		flags.v = __builtin_add_overflow_vf(s1, s2, carry_in);

		return(nz(result));
	}

	__attribute__((always_inline, warn_unused_result))
	uint32_t _subs(const uint32_t s1, const uint32_t s2, const uint32_t carry_in)
	{
		uint32_t result = 0;

		flags.c
			= __builtin_add_overflow(s1, ~s2, &result)
			| __builtin_add_overflow(result, carry_in, &result);

		flags.v = __builtin_sub_overflow_vf(s1, s2, carry_in);

		return(nz(result));
	}

//	__attribute__((always_inline, warn_unused_result))
//	uint32_t add(const uint32_t s1, const uint32_t s2) { return(s1 + s2); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t adds(const uint32_t s1, const uint32_t s2) { return(_adds(s1, s2, 0)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t adc(const uint32_t s1, const uint32_t s2) { return(s1 + s2 + flags.c); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t adcs(const uint32_t s1, const uint32_t s2) { return(_adds(s1, s2, flags.c)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t asr(const uint32_t rm, const uint32_t rs)
	{ flags.sco = asr32_c(rm, rs); return(asr32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t asrs(const uint32_t rm, const uint32_t rs)
	{ flags.c = asr32_c(rm, rs); return(asr32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t lsl(const uint32_t rm, const uint32_t rs)
	{ flags.sco = lsl32_c(rm, rs); return(lsl32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t lsls(const uint32_t rm, const uint32_t rs)
	{ flags.c = lsl32_c(rm, rs); return(lsl32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t lsr(const uint32_t rm, const uint32_t rs)
	{ flags.sco = lsr32_c(rm, rs); return(lsr32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t lsrs(const uint32_t rm, const uint32_t rs)
	{ flags.c = lsr32_c(rm, rs); return(lsr32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t n(const int32_t v) { flags.n = (0 < v); return(v); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t nz(const uint32_t v) { return(n(z(v))); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t nzc(const uint32_t v) { flags.c = flags.sco; return(n(z(v))); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t ror(const uint32_t rm, const uint32_t rs)
	{ flags.sco = ror32_c(rm, rs); return(ror32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t rors(const uint32_t rm, const uint32_t rs)
	{ flags.c = ror32_c(rm, rs); return(ror32(rm, rs)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t rrx(const uint32_t rm)
	{ flags.sco = rrx32_c(rm); return(rrx32_v(rm, flags.c)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t rrxs(const uint32_t rm)
	{ flags.c = rrx32_c(rm); return(rrx32_v(rm, flags.c)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t sbc(const uint32_t s1, const uint32_t s2) { return(s1 - s2 - flags.c); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t sbcs(const uint32_t s1, const uint32_t s2) { return(_subs(s1, s2, flags.c)); }

//	__attribute__((always_inline, warn_unused_result))
//	uint32_t sub(const uint32_t s1, const uint32_t s2) { return(s1 - s2); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t subs(const uint32_t s1, const uint32_t s2) { return(_subs(s1, s2, 1)); }

	__attribute__((always_inline, warn_unused_result))
	uint32_t z(const int32_t v) { flags.z = (0 == v); return(v); }

	struct {
		unsigned raw_flags;
		struct {
			char c:1;
			char n:1;
			char v:1;
			char z:1;
			char sco:1;
		};
	}flags;
}arm_apsr_t;
