#pragma once

/* **** */

#include "git/libbse/include/bitfield.h"

/* **** */

enum {
	ARM_CPSR_M32 = 4,
	ARM_CPSR_T = 5,
	ARM_CPSR_F = 6,
	ARM_CPSR_I = 7,
	ARM_CPSR_A = 8,
	ARM_CPSR_E = 9,
	ARM_CPSR_V = 28,
	ARM_CPSR_C = 29,
	ARM_CPSR_Z = 30,
	ARM_CPSR_N = 31,
//
	ARM_CPSR_Abort = ARM_CPSR_A,
	ARM_CPSR_FIQ = ARM_CPSR_F,
	ARM_CPSR_IRQ = ARM_CPSR_I,
	ARM_CPSR_Thumb = ARM_CPSR_T,
};

enum {
	ARM_CPSR_M_User,
	ARM_CPSR_M_FIQ,
	ARM_CPSR_M_IRQ,
	ARM_CPSR_M_Supervisor,
//
	ARM_CPSR_M_32 = 0x10,
//
	ARM_CPSR_M_Abort = 0x7,
	ARM_CPSR_M_Undefined = 0x8 | 3,
	ARM_CPSR_M_System = 0xf,
};


#define ARM_CPSR(_x) (ARM_CPSR_##_x)
#define ARM_CPSR_BEXT(_x) BEXT(CPSR, ARM_CPSR(_x))
#define ARM_CPSR_BMAS(_x, _set) BMAS(CPSR, ARM_CPSR(_x), _set)

#define IF_CPSR(_x) (0 != ARM_CPSR_BEXT(_x))
#define IF_NOT_CPSR(_x) (0 == ARM_CPSR_BEXT(_x))

#define ARM_CPSR_M(_x) (ARM_CPSR_M_##_x)
#define ARM_CPSR_M32(_x) (ARM_CPSR(M32) | ARM_CPSR_M(_x))
#define ARM_CPSR_M32_BMAS(_x) pbBFINS(CPSR, ARM_CPSR_M32(_x), 0, 5)
