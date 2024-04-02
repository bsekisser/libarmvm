#pragma once

/* **** */

#include "libarm/include/arm_cc.h"

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t _gen_ir_cc(unsigned cc)
{ return(pbBFMOV(cc, 0, 4, 28)); }

static uint32_t _gen_ir_group(unsigned group)
{ return(pbBFMOV(group, 0, 3, 25)); }

static uint32_t _gen_ir_b_offset(int32_t offset)
{ return(mlBFMOVs(offset, 23, 2, 0) & _BM(24)); }

/* **** */

static uint32_t __gen_op_cc(unsigned cc, unsigned group, uint32_t rest)
{
	return(_gen_ir_cc(cc) + _gen_ir_group(group) + rest);
}

/* **** */

static uint32_t _gen_op(unsigned group, uint32_t rest)
{
	GEN_IP = GEN_PC;

	const uint32_t ir = __gen_op_cc(CC_AL, group, rest);
	t->mem[GEN_IP] = htole32(ir);

	GEN_PC += 4;

	return(GEN_IP);
}

static uint32_t _gen_op_cc(unsigned cc, unsigned group, uint32_t rest)
{
	GEN_IP = GEN_PC;

	const uint32_t ir = __gen_op_cc(cc, group, rest);
	t->mem[GEN_IP] = htole32(ir);

	GEN_PC += 4;

	return(GEN_IP);
}
