#pragma once

/* **** */

typedef struct armvm_core_t** armvm_core_h;
typedef struct armvm_core_t* armvm_core_p;

#include "armvm.h"

#include "arm_trace.h" // TODO

/* **** */

#define ARM_VM_GPR(_x) ARM_VM_GPR_##_x

enum {
	ARM_VM_GPR_SP = 13,
	ARM_VM_GPR_LR = 14,
	ARM_VM_GPR_PC = 15,
//
	_ARM_VM_GPR_COUNT_,
};

#define ARM_VM_SPR32(_x) ARM_VM_SPR32_##_x

enum {
	ARM_VM_SPR32_CC,
	ARM_VM_SPR32_CCX,
	ARM_VM_SPR32_CP15R1,
	ARM_VM_SPR32_CPSR,
	ARM_VM_SPR32_IP,
	ARM_VM_SPR32_IR,
//
	_ARM_VM_SPR32_COUNT_,
};

#define ARM_VM_SPR64(_x) ARM_VM_SPR64_##_x

enum {
	ARM_VM_SPR64_CYCLE,
	ARM_VM_SPR64_ICOUNT,
//	ARM_VM_SPR64_IFETCH,
	ARM_VM_SPR64_RESULT,
//
	_ARM_VM_SPR64_COUNT_,
};

typedef struct armvm_core_t {
	uint64_t spr64[_ARM_VM_SPR64_COUNT_];
#define SPR64x(_x) CORE->spr64[_x]

	uint32_t gpr[_ARM_VM_GPR_COUNT_];
#define GPRx(_x) CORE->gpr[_x]

	uint32_t spr32[_ARM_VM_SPR32_COUNT_];
#define SPR32x(_x) CORE->spr32[_x]

	uint32_t* spsr;
//
	arm_trace_p arm_trace;
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
