#pragma once

/* **** */

typedef struct armvm_core_t** armvm_core_h;
typedef struct armvm_core_t* armvm_core_p;

/* **** */

#include "armvm_mmu.h"
#include "armvm_core_config.h"
#include "armvm.h"

/* **** */

#include "armvm_trace.h"

/* **** */

#include <stdint.h>

/* **** */

#define ARMVM_GPR(_x) ARMVM_GPR_##_x

enum {
	ARMVM_GPR_SP = 13,
	ARMVM_GPR_LR = 14,
	ARMVM_GPR_PC = 15,
//
	_ARMVM_GPR_COUNT_,
};

#define ARMVM_SPR32(_x) ARMVM_SPR32_##_x

enum {
	ARMVM_SPR32_CC,
	ARMVM_SPR32_CCX,
	ARMVM_SPR32_CP15R1,
	ARMVM_SPR32_CPSR,
	ARMVM_SPR32_IP,
	ARMVM_SPR32_IR,
//
	_ARMVM_SPR32_COUNT_,
};

#define ARMVM_SPR64(_x) ARMVM_SPR64_##_x

enum {
	ARMVM_SPR64_CYCLE,
	ARMVM_SPR64_ICOUNT,
//	ARMVM_SPR64_IFETCH,
	ARMVM_SPR64_RESULT,
//
	_ARMVM_SPR64_COUNT_,
};

typedef struct armvm_core_t {
	uint64_t spr64[_ARMVM_SPR64_COUNT_];
//
	uint32_t gpr[_ARMVM_GPR_COUNT_];
	uint32_t spr32[_ARMVM_SPR32_COUNT_];
	uint32_t* spsr;
//
	armvm_trace_p armvm_trace;
	armvm_core_config_t config;
//
	armvm_p armvm;
	armvm_core_h h2core;
	armvm_coprocessor_p cp;
	armvm_mmu_p mmu;
}armvm_core_t;

/* **** */

void armvm_core(unsigned action, armvm_core_p core);
armvm_core_p armvm_core_alloc(armvm_core_h h2core, armvm_p avm);

/* **** */

static inline uint32_t spsr(uint32_t* write, armvm_core_p core)
{
	const uint32_t data = core->spsr ? *core->spsr : (write ? *write : 0);

	if(write && core->spsr)
		*core->spsr = *write;

	return(data);
}
