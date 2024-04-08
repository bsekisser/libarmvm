#pragma once

/* **** */

extern "C" {
	#include "libbse/include/unused.h"

	/* **** */

	#include <endian.h>
}

#include "test_gen_utility.h"

/* **** */


static int32_t __b_offset_pc(int32_t bpc)
{ return(bpc - (8 + GEN_PC)); }

static uint32_t _bx_blx_m(int link, arm_reg_t rm)
{
	uint32_t ir = 0x01200010 | (!!link << 5);
		ir |= _gen_ir_rn((arm_reg_t)~0);
		ir |= _gen_ir_rd((arm_reg_t)~0);
		ir |= _gen_ir_rs((arm_reg_t)~0);
		ir |= _gen_ir_rm(rm);

	return(_gen_op(0, ir));
}

static uint32_t _b_bl(int link, uint32_t new_pc)
{
	const uint32_t rest = (!!link << 24)
		| _gen_ir_b_offset(__b_offset_pc(new_pc));

	return(_gen_op(5, rest));
}

UNUSED_FN
static uint32_t b(uint32_t new_pc)
{ return(_b_bl(0, new_pc)); }

UNUSED_FN
static uint32_t bl(uint32_t new_pc)
{ return(_b_bl(1, new_pc)); }

UNUSED_FN
static uint32_t blx(arm_reg_t rm)
{ return(_bx_blx_m(1, rm)); }

UNUSED_FN
static uint32_t blx(uint32_t new_pc)
{
	const uint32_t offset = __b_offset_pc(new_pc);
	const uint32_t h = BMOV(offset, 1, 24);

	const uint32_t rest = h | _gen_ir_b_offset(offset);

	return(_gen_op_cc(CC_NV, 5, rest));
}

UNUSED_FN
static uint32_t bx(arm_reg_t rm)
{ return(_bx_blx_m(0, rm)); }

UNUSED_FN
static uint32_t adcs(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_ADC, rd, rn, rm));
}

UNUSED_FN
static uint32_t add(arm_reg_t rd, arm_reg_t rn, int sop)
{
	return(_gen_op_dp_immediate(ARM_ADD, rd, rn, ROR(sop, 0)));
}

UNUSED_FN
static uint32_t add(arm_reg_t rd, arm_reg_t rn, arm_dp_sop_t sop)
{
	return(_gen_op_dp_sop(ARM_ADD, rd, rn, sop));
}

UNUSED_FN
static uint32_t adds(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_ADD, rd, rn, rm));
}

UNUSED_FN
static uint32_t adds(arm_reg_t rd, arm_reg_t rn, arm_dp_sop_t sop)
{
	return(_gen_op_dps_sop(ARM_ADD, rd, rn, sop));
}

UNUSED_FN
static uint32_t ands(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_AND, rd, rn, rm));
}

UNUSED_FN
static uint32_t asrs(arm_reg_t rd, arm_reg_t rm, int rs)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, ASR(rm, rs)));
}


UNUSED_FN
static uint32_t bics(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_BIC, rd, rn, rm));
}

UNUSED_FN
static uint32_t lsls(arm_reg_t rd, arm_reg_t rm, int rs)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, LSL(rm, rs)));
}

UNUSED_FN
static uint32_t lsrs(arm_reg_t rd, arm_reg_t rm, int rs)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, LSR(rm, rs)));
}

UNUSED_FN
static uint32_t mov(arm_reg_t rd, uint32_t sop)
{
	return(_gen_op_dp_immediate(ARM_MOV, rd, r0, sop));
}

UNUSED_FN
static uint32_t mov(arm_reg_t rd, int sop)
{
	return(_gen_op_dp_immediate(ARM_MOV, rd, r0, ROR(sop, 0)));
}

UNUSED_FN
static uint32_t movs(arm_reg_t rd, uint32_t sop)
{
	return(_gen_op_dps_immediate(ARM_MOV, rd, r0, sop));
}

UNUSED_FN
static uint32_t movs(arm_reg_t rd, uint16_t rm)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, rm));
}

UNUSED_FN
static uint32_t movs(arm_reg_t rd, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, rm));
}

UNUSED_FN
static uint32_t movs(arm_reg_t rd, int sop)
{
	return(_gen_op_dps_immediate(ARM_MOV, rd, r0, ROR(sop, 0)));
}

UNUSED_FN
static uint32_t rors(arm_reg_t rd, arm_reg_t rm, int rs)
{
	return(_gen_op_dps_sop(ARM_MOV, rd, r0, ROR(rm, rs)));
}

UNUSED_FN
static uint32_t rsbs(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_RSB, rd, rn, rm));
}

UNUSED_FN
static uint32_t rsbs(arm_reg_t rd, arm_reg_t rn, arm_dp_sop_t sop)
{
	return(_gen_op_dps_sop(ARM_RSB, rd, rn, sop));
}

UNUSED_FN
static uint32_t subs(arm_reg_t rd, arm_reg_t rn, arm_reg_t rm)
{
	return(_gen_op_dps_sop(ARM_SUB, rd, rn, rm));
}

UNUSED_FN
static uint32_t subs(arm_reg_t rd, arm_reg_t rn, arm_dp_sop_t sop)
{
	return(_gen_op_dps_sop(ARM_SUB, rd, rn, sop));
}
