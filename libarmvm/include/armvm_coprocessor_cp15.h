#pragma once

/* **** */

enum {
	CP15_REG1_BIT_M = 0,
	CP15_REG1_BIT_A = 1,
	CP15_REG1_BIT_V = 13,
	CP15_REG1_BIT_U = 20,
	CP15_REG1_BIT_EE = 25,
};

/* **** */

#include "armvm_coprocessor.h"
#include "armvm_coprocessor_glue.h"
//#include "armvm_core.h"
#include "armvm.h"

/* **** */

#include "libbse/include/bitfield.h"

/* **** */



#define _CP15_REG1_BIT(_x) CP15_REG1_BIT_##_x
#define CP15_REG1_BIT(_x) BEXT(rCP15x(cp15(0, 1, 0, 0), 0), _CP15_REG1_BIT(_x))

static inline void _cp15_reg1_bclr(armvm_coprocessor_p const cp, const unsigned cpx, const unsigned bit)
{ armvm_coprocessor_cp15r_bmas(cp, cpx, bit, 0); }

#define CP15_REG1_BCLR(_x) \
	_cp15_reg1_bclr(pARMVM->coprocessor, cp15(0, 1, 0, 0), _CP15_REG1_BIT(_x))
