#pragma once

/* **** */

#include "armvm_core_shiftbox.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/unused.h"

/* **** */

#include "core_reg.h"

/* **** */

#include <stdint.h>

/* **** */

static void _alubox_flags_nz(armvm_core_ref core, const uint32_t rd)
{
	ARM_CPSR_BMAS(N, BEXT(rd, 31));
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

	const unsigned xvec = (s1 ^ s2);
	const unsigned ovec = (s1 ^ rd) & ~xvec;

	const unsigned cf = BEXT((xvec ^ ovec ^ rd), 31);
	ARM_CPSR_BMAS(C, cf);

	const unsigned vf = BEXT(ovec, 31);
	ARM_CPSR_BMAS(V, vf);
}
