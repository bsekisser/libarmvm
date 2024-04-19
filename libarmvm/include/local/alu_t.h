#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include <stdint.h>

/* **** */

typedef struct alu_t* alu_p;
typedef struct alu_t {
	uint32_t cf, rd, rm, rn, rs, sop;
}alu_t;

typedef uint32_t (*alu_fn)(armvm_core_p const core, alu_p const alu);
