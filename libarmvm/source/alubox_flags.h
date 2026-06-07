#pragma once

/* **** */

#include "armvm_core_shiftbox.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/cpsr.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/bitops32.h"
#include "libbse/include/unused.h"

/* **** */

#include <stdint.h>

/* **** */

static void _alubox_flags_nz(armvm_core_ref core, const int32_t rd)
{
	ARM_CPSR_BMAS(N, 0 > rd);
	ARM_CPSR_BMAS(Z, 0 == rd);
}

UNUSED_FN
static void _alubox_flags_nzc(armvm_core_ref core, const uint32_t rd)
{
	ARM_CPSR_BMAS(C, _shifter_operand_c(core));

	return(_alubox_flags_nz(core, rd));
}

/*
 * Credit to:
 * 		http://www.emulators.com/docs/nx11_flags.htm
 *
 * OF(A+B) = ((A XOR D) AND NOT (A XOR B)) < 0
 * OF(A-B) = ((A XOR D) AND (A XOR B)) < 0
 *
 * CF(A+B) = (((A XOR B) XOR D) < 0) XOR (((A XOR D) AND NOT (A XOR B)) < 0)
 * CF(A-B) = (((A XOR B) XOR D) < 0) XOR (((A XOR D) AND (A XOR B)) < 0)
 *
 */

static void _alubox_flags_x_add_sub(armvm_core_ref core, const uint32_t rd,
	const uint32_t s1, const uint32_t s2)
{
	_alubox_flags_nz(core, rd);

	const int32_t xvec = (s1 ^ s2);
	const int32_t ovec = (s1 ^ rd) & ~xvec;
	const int32_t cvec = xvec ^ ovec ^ rd;

	ARM_CPSR_BMAS(C, 0 > cvec);

	ARM_CPSR_BMAS(V, 0 > ovec);
}
