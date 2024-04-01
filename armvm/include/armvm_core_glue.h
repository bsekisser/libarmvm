#pragma once

/* **** */

#define pARMVM_TRACE pARMVM_CORE->armvm_trace
#define pARMVM_CORE core

#define CONFIG (&pARMVM_CORE->config)

#include "core_trace_glue.h"

/* **** */

#define irGPR(_x) GPRx(ARM_IR_R(_x))

#define irR_NAME(_x) arm_reg_name_lcase_string[0][ARM_IR_R(_x)]

#define SPR64x(_x) pARMVM_CORE->spr64[_x]
#define rSPR64(_x) SPR64x(ARMVM_SPR64(_x))
#define rSPR64hi(_x) ((rSPR64(_x) >> 32) & 0xffff)
#define rSPR64lo(_x) (rSPR64(_x) & 0xffff)

#define CPSR rSPR32(CPSR)
#define pSPSR pARMVM_CORE->spsr

#define CYCLE rSPR64(CYCLE)
#define ICOUNT rSPR64(ICOUNT)

#define IP rSPR32(IP)
#define IR rSPR32(IR)

/* **** */
