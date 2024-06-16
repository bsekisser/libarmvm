#pragma once

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#define ARM_IR_BIT(_x) BEXT(IR, _x)
#define ARM_IR_CC mlBFEXT(IR, 31, 28)
#define ARM_IR_GROUP mlBFEXT(IR, 27, 25)
#define ARM_IR_RD mlBFEXT(IR, 15, 12)
#define ARM_IR_RN mlBFEXT(IR, 19, 16)
#define ARM_IR_RS mlBFEXT(IR, 11, 8)
#define ARM_IR_RM mlBFEXT(IR, 3, 0)
#define ARM_IR_SHIFT_OPERAND mlBFEXT(IR, 11, 0)

#define ARM_IR_R(_x) (ARM_IR_R##_x)

#define ARM_IR_RDHi ARM_IR_RN
#define ARM_IR_RDLo ARM_IR_RD

#define ARM_IR_B_LINK BEXT(IR, 24)
#define ARM_IR_B_OFFSET mlBFMOVs(IR, 23, 0, 2)

#define ARM_IR_BLX_H BEXT(IR, 24)
#define ARM_IR_BLX_OFFSET (mlBFMOVs(IR, 23, 0, 2) | (ARM_IR_BLX_H << 1))

#define ARM_IR_DP_I BEXT(IR, 4)
#define ARM_IR_DP_OPCODE mlBFEXT(IR, 24, 21)
#define ARM_IR_DP_OPCODES mlBFEXT(IR, 24, 20)
#define ARM_IR_DP_S BEXT(IR, 20)
#define ARM_IR_DP_SHIFT_TYPE mlBFEXT(IR, 6, 5)
#define ARM_IR_DP_SHIFT_AMOUNT mlBFEXT(IR, 11, 7)
#define ARM_IR_DP_WB (2 != mlBFEXT(IR, 24, 23))

#define ARM_IR_DPI BEXT(IR, 25)
#define ARM_IR_DPI_ROTATE_AMOUNT mlBFMOV(IR, 11, 8, 1)
#define ARM_IR_DPI_IMMEDIATE mlBFEXT(IR, 7, 0)

enum {
	ARM_IR_LDST_BIT_I = 25,
	ARM_IR_LDST_BIT_P = 24,
	ARM_IR_LDST_BIT_U = 23,
	ARM_IR_LDST_BIT_B = 22,
	ARM_IR_LDST_BIT_W = 21,
	ARM_IR_LDST_BIT_L = 20,
};

#define ARM_IR_LDST_BIT(_x) BEXT(IR, ARM_IR_LDST_BIT_##_x)
#define ARM_IR_LDST_FLAG_T (!ARM_IR_LDST_BIT(P) && ARM_IR_LDST_BIT(W))
#define ARM_IR_LDST_IMMEDIATE_OFFSET mlBFEXT(IR, 11, 0)
#define ARM_IR_LDST_IPUBWL mlBFEXT(IR, 25, 20)

enum {
	ARM_IR_LDSTM_BIT_S = 22,
};

#define ARM_IR_LDSTM_BIT(_x) BEXT(IR, ARM_IR_LDSTM_BIT_##_x)

enum {
	ARM_IR_LDST_SH_BIT_I = 22,
	ARM_IR_LDST_SH_BIT_S = 6,
	ARM_IR_LDST_SH_BIT_H = 5,
};

#define ARM_IR_LDST_SH_BIT(_x) BEXT(IR, ARM_IR_LDST_SH_BIT_ ## _x)

#define ARM_IR_LDST_SH_FLAG_B (ARM_IR_LDST_SH_FLAG_S && !ARM_IR_LDST_SH_BIT(H))
#define ARM_IR_LDST_SH_FLAG_D (!ARM_IR_LDST_BIT(L) && ARM_IR_LDST_SH_BIT(S))
#define ARM_IR_LDST_SH_FLAG_H (ARM_IR_LDST_SH_BIT(H) && !ARM_IR_LDST_SH_FLAG_D)
#define ARM_IR_LDST_SH_FLAG_L (ARM_IR_LDST_BIT(L) || (!ARM_IR_LDST_BIT(L) && !ARM_IR_LDST_SH_BIT(H)))
#define ARM_IR_LDST_SH_FLAG_S (ARM_IR_LDST_BIT(L) && ARM_IR_LDST_SH_BIT(S))

#define ARM_IR_LDST_SH_OFFSET (ARM_IR_RM + (ARM_IR_RS << 4))

#define ARM_IR_MCRC_CPx ARM_IR_RS
#define ARM_IR_MCRC_CRm ARM_IR_RM
#define ARM_IR_MCRC_CRn ARM_IR_RN
#define ARM_IR_MCRC_L BEXT(IR, 20)
#define ARM_IR_MCRC_OP1 mlBFEXT(IR, 23, 21)
#define ARM_IR_MCRC_OP2 mlBFEXT(IR, 7, 5)

#define ARM_IR_MRSR_R BEXT(IR, 22)
