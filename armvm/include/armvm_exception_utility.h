#pragma once

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_core.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t _high_vectors(armvm_core_p core)
{ return(CP15_REG1_BIT(V) ? (~0xffff) : 0); }
