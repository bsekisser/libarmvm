#pragma once

/* **** */

typedef uint16_t armcc_sop_t;

extern "C" {
	#include "armcc_t.h"
}

/* **** */

extern "C" {
	#include "libbse/include/bitfield.h"
}

/* **** */

#include <stdint.h>

/* **** */

static armcc_sop_t _gen_arm_sop_shift_immediate(const arm_sop_t shift_type,
	const arm_reg_t rm, const uint8_t rs)
{
	const armcc_sop_t sop = pbBFMOV(rs, 0, 5, 7)
		| pbBFMOV(shift_type, 0, 2, 5)
		| gen_arm_ir_rm(rm);

	return(sop);
}

static armcc_sop_t _gen_arm_sop_shift_immediate(const arm_sop_t shift_type,
	const arm_reg_t rm, const arm_reg_t rs)
{
	const armcc_sop_t sop = gen_arm_ir_rs(rs)
		| pbBFMOV(shift_type, 0, 2, 5)
		| gen_arm_ir_rm(rm);

	return(sop);
}

static armcc_sop_t gen_arm_dp_sop_lsl(const arm_reg_t rm, const int rs)
{ return(_gen_arm_sop_shift_immediate(ARM_SOP_LSL, rm, rs)); }

static armcc_sop_t gen_arm_dp_sop_ror_i(const uint8_t imm, const uint8_t rs)
{ return((imm & 0xff) | pbBFMOV(rs, 1, 4, 8)); }
