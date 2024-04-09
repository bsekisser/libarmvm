#pragma once

/* **** */

extern "C" {
	#include "libarm/include/arm_cc.h"

/* **** */

	#include "libbse/include/bitfield.h"

/* **** */

	#include <stdint.h>
}

typedef uint32_t arm_dpi_sop_t;
typedef uint16_t arm_dp_sop_t;
typedef uint16_t arm_sop_t;

extern test_p t;

#include "arm_gen_ir.hpp"
#include "arm_gen_sop.hpp"

/* **** */

static uint32_t __gen_op_cc(unsigned cc, unsigned group, uint32_t rest)
{
	GEN_IP = GEN_PC;

	const uint32_t ir = _gen_ir_cc(cc) | _gen_ir_group(group) | rest;

	*(uint32_t*)(t->mem + GEN_IP) = htole32(ir);

	GEN_COUNT++;
	GEN_PC += 4;

	return(ir);
}

/* **** */

static uint32_t _gen_op(unsigned group, uint32_t rest)
{
	__gen_op_cc(CC_AL, group, rest);
	return(GEN_IP);
}

UNUSED_FN
static uint32_t _gen_op_cc(unsigned cc, unsigned group, uint32_t rest)
{
	__gen_op_cc(cc, group, rest);
	return(GEN_IP);
}

static uint32_t _gen_op_dp(unsigned group, unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint32_t sop)
{
	__gen_op_cc(CC_AL, group,
		_gen_ir_dp_op(dp_op)
		| _gen_ir_rd(rd)
		| _gen_ir_rn(rn)
		| _gen_ir_dp_sop(sop));

	return(GEN_IP);
}

static uint32_t _gen_op_dps(unsigned group, unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint32_t sop)
{
	__gen_op_cc(CC_AL, group,
		_gen_ir_dps_op(dp_op)
		| _gen_ir_rd(rd)
		| _gen_ir_rn(rn)
		| _gen_ir_dp_sop(sop));

	return(GEN_IP);
}


static uint32_t _gen_op_dp_immediate(unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint32_t sop)
{ return(_gen_op_dp(1, dp_op, rd, rn, sop)); }

static uint32_t _gen_op_dp_sop(unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint16_t sop)
{ return(_gen_op_dp(0, dp_op, rd, rn, sop)); }

static uint32_t _gen_op_dps_immediate(unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint32_t sop)
{ return(_gen_op_dps(1, dp_op, rd, rn, sop)); }

static uint32_t _gen_op_dps_sop(unsigned dp_op, arm_reg_t rd, arm_reg_t rn, uint16_t sop)
{ return(_gen_op_dps(0, dp_op, rd, rn, sop)); }
