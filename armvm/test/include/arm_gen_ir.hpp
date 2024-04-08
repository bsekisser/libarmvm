#pragma once

/* **** */

extern "C" {
	#include "libbse/include/bitfield.h"

	#include <stdint.h>
}

/* **** */

static uint32_t _gen_ir_cc(unsigned cc)
{ return(pbBFMOV(cc, 0, 4, 28)); }

static uint32_t _gen_ir_dp_op(unsigned dp_op)
{ return(pbBFMOV(dp_op, 0, 4, 21)); }

static uint32_t _gen_ir_dp_shift_immediate(unsigned rs)
{ return(pbBFMOV(rs, 0, 5, 7)); }

static uint32_t _gen_ir_dp_shift_type(unsigned type)
{ return(pbBFMOV(type, 0, 2, 5)); }

static uint32_t _gen_ir_dp_sop(uint32_t sop)
{ return(BTST(sop, 25) | pbBFTST(sop, 0, 12)); }

static uint32_t _gen_ir_dpi_immediate(unsigned rm)
{ return(pbBFEXT(rm, 0, 8)); }

static uint32_t _gen_ir_dpi_rotate(unsigned rs)
{ return(pbBFMOV(rs, 1, 4, 8)); }

static uint32_t _gen_ir_dps_op(unsigned dp_op)
{ return(pbBFMOV(dp_op, 0, 4, 21) | _BV(20)); }

static uint32_t _gen_ir_group(unsigned group)
{ return(pbBFMOV(group, 0, 3, 25)); }

static uint32_t _gen_ir_b_offset(int32_t offset)
{ return(mlBFMOVs(offset, 23, 2, 0) & _BM(24)); }

static uint32_t _gen_ir_rd(arm_reg_t rd)
{ return(pbBFMOV(rd, 0, 4, 12)); }

static uint32_t _gen_ir_rm(arm_reg_t rm)
{ return(pbBFEXT(rm, 0, 4)); }

static uint32_t _gen_ir_rn(arm_reg_t rn)
{ return(pbBFMOV(rn, 0, 4, 16)); }

static uint32_t _gen_ir_rs(arm_reg_t rs)
{ return(pbBFMOV(rs, 0, 4, 8)); }

