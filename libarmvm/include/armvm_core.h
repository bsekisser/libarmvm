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
	ARMVM_SPR32_ABT_R14,
	ARMVM_SPR32_ABT_SPSR,
	ARMVM_SPR32_SVC_R14,
	ARMVM_SPR32_SVC_SPSR,
	ARMVM_SPR32_UND_R14,
	ARMVM_SPR32_UND_SPSR,

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
#define SPR64x(_x) pARMVM_CORE->spr64[_x]
#define rSPR64(_x) SPR64x(ARMVM_SPR64(_x))
#define rSPR64hi(_x) ((rSPR64(_x) >> 32) & 0xffff)
#define rSPR64lo(_x) (rSPR64(_x) & 0xffff)
//
	uint32_t gpr[_ARMVM_GPR_COUNT_];
#define GPRx(_x) pARMVM_CORE->gpr[_x]
#define irGPR(_x) GPRx(ARM_IR_R(_x))
#define vmGPR(_x) GPRx(ARMVM_GPR(_x))

	uint32_t spr32[_ARMVM_SPR32_COUNT_];
#define SPR32x(_x) pARMVM_CORE->spr32[_x]
#define rSPR32(_x) SPR32x(ARMVM_SPR32(_x))

	uint32_t* spsr;
#define pSPSR pARMVM_CORE->spsr
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

void armvm_core(const unsigned action, armvm_core_p const core);
armvm_core_p armvm_core_alloc(armvm_core_h const h2core, armvm_p const avm);
void armvm_core_step(armvm_core_p const core);

/* **** */

static inline uint32_t spsr(uint32_t *const write, armvm_core_p core)
{
	const uint32_t data = core->spsr ? *core->spsr : (write ? *write : 0);

	if(write && core->spsr)
		*core->spsr = *write;

	return(data);
}