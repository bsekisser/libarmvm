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
	if(APSRp_FLAGS(set)) { // terminal function, clear set unless sticky.
		APSRp_FLAGS(set) &= APSRp_FLAGS(sticky);

		APSRp_SET(n) = (0 > rd);
		APSRp_SET(z) = (0 == rd);
	}

	return(rd);
}

static __attribute__((warn_unused_result))
uint32_t __flags_nzc(arm_apsr_ref apsr, const int32_t rd)
{
	if(APSRp_FLAGS(set))
		APSRp_SET(c) = APSRp_FLAGS(sco);

	return(__flags_nz(apsr, rd));
}

/* **** */

static __attribute__((warn_unused_result))
uint32_t _flags_add(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop, const uint32_t carry_in)
{
	uint32_t result = 0;

	const unsigned cf = __builtin_add_overflow(rn, sop, &result)
			| __builtin_add_overflow(result, carry_in, &result);

	if(APSRp_FLAGS(set)) {
		APSRp_SET(c) = cf;
		APSRp_SET(v) = __builtin_add_overflow_vf(rn, sop, carry_in);
	}

	return(__flags_nz(apsr, result));
}

static __attribute__((warn_unused_result))
uint32_t _flags_sub(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop, const uint32_t carry_in)
{
	uint32_t result = 0;

	const unsigned cf = __builtin_add_overflow(rn, ~sop, &result)
		| __builtin_add_overflow(result, carry_in, &result);

	if(APSRp_FLAGS(set)) {
		APSRp_SET(c) = cf;
		APSRp_SET(v) = __builtin_sub_overflow_vf(rn, sop, carry_in);
	}

	return(__flags_nz(apsr, result));
}

/* **** */

inline __attribute__((visibility("default")))
uint32_t arm_apsr_flags_nz(arm_apsr_ref apsr, const uint32_t rd)
{ return(__flags_nz(apsr, rd)); }

inline __attribute__((visibility("default")))
uint32_t arm_apsr_flags_nzc(arm_apsr_ref apsr, const uint32_t rd)
{ return(__flags_nzc(apsr, rd)); }

static __attribute__((warn_unused_result))
uint32_t flags_adc(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_add(apsr, rn, sop, APSRp(c))); }

static __attribute__((warn_unused_result))
uint32_t flags_add(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_add(apsr, rn, sop, 0)); }

//static __attribute__((warn_unused_result))
//uint32_t flags_nz(arm_apsr_ref apsr, const int32_t rd)
//{ return(arm_apsr_flags_nz(apsr, rd)); }

static __attribute__((warn_unused_result))
uint32_t flags_nzc(arm_apsr_ref apsr, const int32_t rd)
{ return(__flags_nzc(apsr, rd)); }

static __attribute__((warn_unused_result))
uint32_t flags_sbc(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(apsr, rn, sop, APSRp(c))); }

static __attribute__((warn_unused_result))
uint32_t flags_sub(arm_apsr_ref apsr, const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(apsr, rn, sop, 1)); }
