#pragma once

/* **** */

#include "apsr/apsr.h"
#include "apsr/glue.h"

/* **** */

#include <stdint.h>

/* **** */

static __attribute__((warn_unused_result))
uint32_t __builtin_add_overflow_vf(const int32_t rn, const int32_t sop, int32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_add_overflow(rn, sop, &result)
		| __builtin_add_overflow(result, carry_in, &result));
}

static __attribute__((warn_unused_result))
uint32_t __builtin_sub_overflow_vf(const int32_t rn, const int32_t sop, int32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_sub_overflow(rn, sop, &result)
		| __builtin_sub_overflow(result, carry_in, &result));
}

static __attribute__((warn_unused_result))
uint32_t __flags_nz(arm_apsr_ref apsr, const int32_t rd)
{
	APSRp_OUT(n) = (0 > rd);
	APSRp_OUT(z) = (0 == rd);

	return(rd);
}

static __attribute__((warn_unused_result))
uint32_t _flags_add(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop, const uint32_t carry_in)
{
	uint32_t result = 0;

	APSRp_OUT(c) = __builtin_add_overflow(rn, sop, &result)
		| __builtin_add_overflow(result, carry_in, &result);

	APSRp_OUT(v) = __builtin_add_overflow_vf(rn, sop, carry_in);

	return(__flags_nz(apsr, result));
}

static __attribute__((warn_unused_result))
uint32_t _flags_sub(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop, const uint32_t carry_in)
{
	uint32_t result = 0;

	APSRp_OUT(c) = __builtin_add_overflow(rn, ~sop, &result)
		| __builtin_add_overflow(result, carry_in, &result);

	APSRp_OUT(v) = __builtin_sub_overflow_vf(rn, sop, carry_in);

	return(__flags_nz(apsr, result));
}

static __attribute__((warn_unused_result))
uint32_t flags_adc(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_add(apsr, rn, sop, APSRp(c))); }

static __attribute__((warn_unused_result))
uint32_t flags_add(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_add(apsr, rn, sop, 0)); }

static __attribute__((warn_unused_result))
uint32_t flags_nz(arm_apsr_ref apsr, const int32_t rd)
{ return(__flags_nz(apsr, rd)); }

static __attribute__((warn_unused_result))
uint32_t flags_sbc(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(apsr, rn, sop, APSRp(c))); }

static __attribute__((warn_unused_result))
uint32_t flags_sub(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(apsr, rn, sop, 1)); }
