#pragma once

/* **** */

#include "armvm_core_shiftbox.h"
#include "armvm_core.h"
#include "reg.h"

/* **** */

#include "libarm/include/cpsr.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/bitops32.h"
#include "libbse/include/unused.h"

/* **** */

#include <stdint.h>

/* **** */

static
unsigned __builtin_add_overflow_vf(int32_t s1, int32_t s2, int32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_add_overflow(s1, s2, &result)
		| __builtin_add_overflow(result, carry_in, &result));
}

static
unsigned __builtin_sub_overflow_vf(int32_t s1, int32_t s2, int32_t carry_in)
{
	int32_t result = 0;

	return(__builtin_sub_overflow(s1, s2, &result)
		| __builtin_sub_overflow(result, carry_in, &result));
}

static // used directly at parts, no s or pc check
uint32_t __flags_nz(armvm_core_ref core, const int32_t rd)
{
	ARM_CPSR_BMAS(N, 0 > rd);
	ARM_CPSR_BMAS(Z, 0 == rd);

	return(rd);
}

/* **** */

static
uint32_t _flags_add(armvm_core_ref core, const unsigned s,
	const uint32_t s1, const uint32_t s2, const unsigned c)
{
	const uint32_t carry_in = c && IF_CPSR(C);
	uint32_t result = 0;

	const unsigned cf
		= __builtin_add_overflow(s1, s2, &result)
		| __builtin_add_overflow(result, carry_in, &result);

	if(s) {
		ARM_CPSR_BMAS(C, cf);
		ARM_CPSR_BMAS(V, __builtin_add_overflow_vf(s1, s2, carry_in));
		return(__flags_nz(core, result));
	}

	return(result);
}

static
uint32_t _flags_sub(armvm_core_ref core, const unsigned s,
	const uint32_t s1, const uint32_t s2, const unsigned c)
{
	const uint32_t carry_in = c ? IF_CPSR(C) : 1;
	uint32_t result = 0;

	const unsigned cf
		= __builtin_add_overflow(s1, ~s2, &result)
		| __builtin_add_overflow(result, carry_in, &result);

	if(s) {
		ARM_CPSR_BMAS(C, cf);
		ARM_CPSR_BMAS(V, __builtin_sub_overflow_vf(s1, s2, carry_in));
		return(__flags_nz(core, result));
	}

	return(result);
}

/* **** */

static
uint32_t flags_adc(armvm_core_ref core, const unsigned s,
	const uint32_t rn, const uint32_t sop)
{ return(_flags_add(core, s, rn, sop, 1)); }

static
uint32_t flags_add(armvm_core_ref core, const unsigned s,
	const uint32_t rn, const uint32_t sop)
{ return(_flags_add(core, s, rn, sop, 0)); }

static
uint32_t flags_nz(armvm_core_ref core, const unsigned s,
	const uint32_t rd)
{ return(s ? __flags_nz(core, rd) : rd); }

static
uint32_t flags_nzc(armvm_core_ref core, const unsigned s,
	const uint32_t rd)
{
	if(s) {
		ARM_CPSR_BMAS(C, _shifter_operand_c(core));
		return(__flags_nz(core, rd));
	}

	return(rd);
}

static
uint32_t flags_sbc(armvm_core_ref core, const unsigned s,
	const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(core, s, rn, sop, 1)); }

static
uint32_t flags_sub(armvm_core_ref core, const unsigned s,
	const uint32_t rn, const uint32_t sop)
{ return(_flags_sub(core, s, rn, sop, 0)); }
