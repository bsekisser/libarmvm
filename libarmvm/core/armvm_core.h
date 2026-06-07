#pragma once

/* **** */

typedef struct armvm_core_tag** armvm_core_hptr;
typedef armvm_core_hptr const armvm_core_href;

typedef struct armvm_core_tag* armvm_core_ptr;
typedef armvm_core_ptr const armvm_core_ref;

/* **** */

#include "armvm_mmu.h"
#include "armvm_coprocessor.h"
#include "armvm_core_config.h"
#include "armvm.h"

/* **** */

#include "libarm/include/cpsr.h"

#include "libbse/include/action.h"

/* **** */

#include <stdint.h>

/* **** */

typedef enum gpr_enum {
	r0, r1, r2, r3, r4, r5, r6, r7,
	r8, r9,r10,r11,r12,r13,r14,r15,
//
	rLR = r14,
	rPC = r15,
	rSP = r13,
}gpr_enum;

typedef enum reg_enum {
	rRD = 16, rRM, rRN, rRS,
//
	rRDHi, rREA, rRSOP,
//
	__REG_COUNT__,
//
	rRDLo = rRD,
}reg_enum;

typedef struct reg_tag {
	gpr_enum r;
	uint32_t v;
}reg_t;

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
	ARMVM_SPR32_IP,
	ARMVM_SPR32_IR,
//
	_ARMVM_SPR32_COUNT_,
};

#define rABT(_x) rSPR32(ABT_##_x)
#define rFIQ(_x) rSPR32(FIQ_##_x)
#define rIRQ(_x) rSPR32(IRQ_##_x)
#define rSVC(_x) rSPR32(SVC_##_x)
#define rUND(_x) rSPR32(UND_##_x)

#define ARMVM_SPR64(_x) ARMVM_SPR64_##_x

enum {
	ARMVM_SPR64_CYCLE,
	ARMVM_SPR64_ICOUNT,
	ARMVM_SPR64_RESULT,
//
	_ARMVM_SPR64_COUNT_,
};

typedef struct armvm_core_state_flags_tag* armvm_core_state_flags_ptr;
typedef armvm_core_state_flags_ptr const armvm_core_state_flags_ref;

typedef struct armvm_core_state_flags_tag {
	unsigned crashed:1;
	unsigned halt:1;
	unsigned pause:1;
	unsigned paused:1;
	unsigned thread_running:1;
}armvm_core_state_flags_t;

typedef struct armvm_core_tag {
	uint32_t gpr[16];
#define GPRx(_x) pCORE->gpr[_x & 15]
#define irGPR(_x) GPRx(ARM_IR_R(_x))
#define vGPR(_) GPRx(r##_)
//
	reg_t reg[__REG_COUNT__];
#define xR(_) (&pCORE->reg[_ - 16])
#define xRr(_) xR(rR##_)
#define vR(_) xRr(_)->v
#define vRx(_) xR(_)->v
#define rR(_) xRr(_)->r
#define rRx(_) xR(_)->r
//
	uint64_t spr64[_ARMVM_SPR64_COUNT_];
#define SPR64x(_x) pCORE->spr64[_x]
#define rSPR64(_x) SPR64x(ARMVM_SPR64(_x))
#define rSPR64hi(_x) ((rSPR64(_x) >> 32) & 0xffff)
#define rSPR64lo(_x) (rSPR64(_x) & 0xffff)
//
	uint32_t spr32[_ARMVM_SPR32_COUNT_];
#define SPR32x(_x) pCORE->spr32[_x]
#define rSPR32(_x) SPR32x(ARMVM_SPR32(_x))

	uint32_t* spsr;
#define pSPSR pCORE->spsr
//
	armvm_core_config_t config;
#define CONFIG (&pCORE->config)
//
	armvm_core_state_flags_t flags;

	libarmvm_ptr armvm;
	armvm_coprocessor_ptr cp;
	armvm_mmu_ptr mmu;
}armvm_core_t;

#define ARM_IP_NEXT ((4 + IP) & ~3)
#define ARM_PC_NEXT ((4 + ARM_IP_NEXT) & ~3)
#define CCX rSPR32(CCX)
#define CPSR rSPR32(CPSR)
#define CYCLE rSPR64(CYCLE)
#define ICOUNT rSPR64(ICOUNT)
#define LR vGPR(LR)
#define IP rSPR32(IP)
#define IR rSPR32(IR)
#define PC vGPR(PC)
#define SP vGPR(SP)
#define THUMB_IP_NEXT ((2 + IP) & ~1)
#define THUMB_PC_NEXT ((2 + THUMB_IP_NEXT) & ~1)
#define irR_NAME(_x) rR_NAMEx(ARM_IR_R(_x))
#define rR_IS_NOT_PC(_x) (rPC != ARM_IR_R(_x))
#define rR_IS_PC(_x) (rPC == ARM_IR_R(_x))
#define rR_NAME(_x) rR_NAMEx(rR(_x))
#define rR_NAMEx(_x) arm_reg_name_lcase_string[0][_x]

/* **** */

extern action_list_t armvm_core_action_list;

//__attribute__((warn_unused_result))
armvm_core_ptr armvm_core_alloc(libarmvm_ref avm, armvm_core_href h2core);

__attribute__((warn_unused_result))
int armvm_core_in_a_privaleged_mode(armvm_core_ref core);

//__attribute__((warn_unused_result))
int armvm_core_pcx(armvm_core_ref core, const uint32_t new_pc);

//__attribute__((warn_unused_result))
int armvm_core_pcx_v5(armvm_core_ref core, const uint32_t new_pc);

void armvm_core_psr_mode_switch(armvm_core_ref core, const uint32_t new_cpsr);
void armvm_core_psr_mode_switch_cpsr(armvm_core_ref core, const uint32_t new_cpsr);
void armvm_core_psr_mode_switch_cpsr_spsr(armvm_core_ref core);

//__attribute__((warn_unused_result))
uint32_t armvm_core_reg_user(armvm_core_ref core, const unsigned r, uint32_t *const v);

//__attribute__((warn_unused_result))
uint32_t armvm_core_spsr(armvm_core_ref core, uint32_t *const write);

//__attribute__((warn_unused_result))
int armvm_core_step(armvm_core_ref core);

//__attribute__((warn_unused_result))
void* armvm_core_threaded_run(armvm_core_ref core);

/* **** */

#define UNPREDICTABLE
