#pragma once

/* **** */

#include "libbse/include/bitfield.h"

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

#define ARM_CPSRx_BEXT(_psr, _x) BEXT(_psr, ARM_CPSR(_x))

static inline uint32_t __arm_bic(const uint32_t data, const uint32_t mask, const uint32_t set)
{ return((data & ~mask) | (set & mask)); }

#define ARM_CPSRx_BIC(_psr, _mask, _set) _psr = __arm_bic(_psr, _mask, _set)
#define ARM_CPSRx_BMAS(_psr, _x, _set) BMAS(_psr, ARM_CPSR(_x), _set)
#define ARM_CPSRx_BSET(_psr, _x) BSET(_psr, ARM_CPSR(_x))

#define ARM_CPSR_BEXT(_x) ARM_CPSRx_BEXT(CPSR, _x)
#define ARM_CPSR_BIC(_mask, _set) ARM_CPSRx_BIC(CPSR, _mask, _set)
#define ARM_CPSR_BMAS(_x, _set) ARM_CPSRx_BMAS(CPSR, _x, _set)
#define ARM_CPSR_BSET(_x) ARM_CPSRx_BSET(CPSR, _x)

#define ARM_CPSR_MASK_NZ \
	(_BV(ARM_CPSR(N)) | _BV(ARM_CPSR(Z)))

#define ARM_CPSR_MASK_NZC \
	(ARM_CPSR_MASK_NZ | _BV(ARM_CPSR(C)))

#define ARM_CPSR_MASK_NZCV \
	(ARM_CPSR_MASK_NZC | _BV(ARM_CPSR(V)))

#define IF_CPSR(_x) (0 != ARM_CPSR_BEXT(_x))
#define IF_NOT_CPSR(_x) (0 == ARM_CPSR_BEXT(_x))

#define ARM_CPSR_M(_x) (ARM_CPSR_M_##_x)
#define ARM_CPSR_M32(_x) (ARM_CPSR_M(32) | ARM_CPSR_M(_x))
#define ARM_CPSR_M32_BMAS(_x) pbBFINS(CPSR, ARM_CPSR_M32(_x), 0, 5)
