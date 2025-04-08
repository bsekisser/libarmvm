#pragma once

/* **** */

typedef struct armvm_core_t** armvm_core_h;
typedef armvm_core_h const armvm_core_href;

typedef struct armvm_core_t* armvm_core_p;
typedef armvm_core_p const armvm_core_ref;

/* **** */

#include "armvm_mmu.h"
#include "armvm_coprocessor.h"
#include "armvm_core_config.h"
#include "armvm.h"

/* **** */

#include "armvm_trace.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

#include "libbse/include/action.h"

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
	ARMVM_SPR32_ABT_R13,
	ARMVM_SPR32_ABT_R14,
	ARMVM_SPR32_ABT_SPSR,
	ARMVM_SPR32_IRQ_R13,
	ARMVM_SPR32_IRQ_R14,
	ARMVM_SPR32_IRQ_SPSR,
	ARMVM_SPR32_FIQ_R8,
	ARMVM_SPR32_FIQ_R9,
	ARMVM_SPR32_FIQ_R10,
	ARMVM_SPR32_FIQ_R11,
	ARMVM_SPR32_FIQ_R12,
	ARMVM_SPR32_FIQ_R13,
	ARMVM_SPR32_FIQ_R14,
	ARMVM_SPR32_FIQ_SPSR,
	ARMVM_SPR32_SVC_R13,
	ARMVM_SPR32_SVC_R14,
	ARMVM_SPR32_SVC_SPSR,
	ARMVM_SPR32_UND_R13,
	ARMVM_SPR32_UND_R14,
	ARMVM_SPR32_UND_SPSR,
//
	ARMVM_SPR32_CC,
	ARMVM_SPR32_CCX,
//	ARMVM_SPR32_CP15R1,
	ARMVM_SPR32_CPSR,
//	ARMVM_SPR32_EA,
	ARMVM_SPR32_IP,
	ARMVM_SPR32_IR,
//	ARMVM_SPR32_RD,
//	ARMVM_SPR32_REA,
//	ARMVM_SPR32_RM,
//	ARMVM_SPR32_RN,
//	ARMVM_SPR32_RS,
//	ARMVM_SPR32_RSOP,
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
	armvm_trace_t armvm_trace;
	armvm_core_config_t config;
//
	armvm_p armvm;
	armvm_core_h h2core;
	armvm_coprocessor_p cp;
	armvm_mmu_ptr mmu;
}armvm_core_t;

/* **** */

void armvm_core(armvm_core_p const core, action_ref action);
armvm_core_p armvm_core_alloc(armvm_p const avm, armvm_core_h const h2core);
int armvm_core_in_a_privaleged_mode(armvm_core_p const core);
int armvm_core_pcx(armvm_core_p const core, const uint32_t new_pc);
int armvm_core_pcx_v5(armvm_core_p const core, const uint32_t new_pc);
void armvm_core_psr_mode_switch(armvm_core_p const core, const uint32_t new_cpsr);
void armvm_core_psr_mode_switch_cpsr(armvm_core_p const core, const uint32_t new_cpsr);
void armvm_core_psr_mode_switch_cpsr_spsr(armvm_core_p const core);
uint32_t armvm_core_reg_user(armvm_core_p const core, const unsigned r, uint32_t *const v);
uint32_t armvm_core_spsr(armvm_core_p const core, uint32_t *const write);
int armvm_core_step(armvm_core_p const core);

/* **** */

#define UNPREDICTABLE
