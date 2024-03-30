#pragma once

/* **** */

enum {
	CP15_REG1_BIT_U = 20,
};

/* **** */

#include "armvm_core_glue.h"

/* **** */

#include "armvm.h"

/* **** */

#define _CP15_REG1_BIT(_x) CP15_REG1_BIT_##_x
#define CP15_REG1_BIT(_x) BEXT(rSPR32(CP15R1), _CP15_REG1_BIT(_x))

