#pragma once

/* **** */

#include "arm_gen_ir.hpp"
#include "test_gen_utility.h"

/* **** */

static uint32_t _gen_op_dp_sop_ror(int rm, int rs)
{ return(_BV(25) | _gen_ir_dpi_immediate(rm) | _gen_ir_dpi_rotate(rs)); }

static uint16_t _gen_op_dp_sop_shift(arm_reg_t rm, unsigned type, arm_reg_t rs)
{ return(_gen_ir_rm(rm) | _gen_ir_dp_shift_type(type) | _BV(4) | _gen_ir_rs(rs)); }

static uint16_t _gen_op_dp_sop_shift_immediate(arm_reg_t rm, unsigned type, int rs)
{ return(_gen_ir_rm(rm) | _gen_ir_dp_shift_type(type) | _gen_ir_dp_shift_immediate(rs)); }

/* **** */

UNUSED_FN
static uint16_t ASR(arm_reg_t rm, int rs)
{ return(_gen_op_dp_sop_shift_immediate(rm, ARM_SOP_ASR, rs)); }

UNUSED_FN
static uint16_t ASR(arm_reg_t rm, arm_reg_t rs)
{ return(_gen_op_dp_sop_shift(rm, ARM_SOP_ASR, rs)); }

UNUSED_FN
static uint16_t LSL(arm_reg_t rm, int rs)
{ return(_gen_op_dp_sop_shift_immediate(rm, ARM_SOP_LSL, rs)); }

UNUSED_FN
static uint16_t LSL(arm_reg_t rm, arm_reg_t rs)
{ return(_gen_op_dp_sop_shift(rm, ARM_SOP_LSL, rs)); }

UNUSED_FN
static uint16_t LSR(arm_reg_t rm, int rs)
{ return(_gen_op_dp_sop_shift_immediate(rm, ARM_SOP_LSR, rs)); }

UNUSED_FN
static uint16_t LSR(arm_reg_t rm, arm_reg_t rs)
{ return(_gen_op_dp_sop_shift(rm, ARM_SOP_LSR, rs)); }

UNUSED_FN
static uint16_t ROR(arm_reg_t rm, int rs)
{ return(_gen_op_dp_sop_shift_immediate(rm, ARM_SOP_ROR, rs)); }

UNUSED_FN
static uint32_t ROR(int rm, int rs)
{ return(_gen_op_dp_sop_ror(rm, rs)); }

UNUSED_FN
static uint16_t ROR(arm_reg_t rm, arm_reg_t rs)
{ return(_gen_op_dp_sop_shift(rm, ARM_SOP_ROR, rs)); }
