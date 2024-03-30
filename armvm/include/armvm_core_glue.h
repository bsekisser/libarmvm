#pragma once

/* **** */

#define pARM_TRACE (pARM_VM->arm_trace)
#define pARM_VM avm

#define vmCONFIG (&pARM_VM->arm_config)

#include "core_trace_glue.h"

/* **** */

#define rGPR(_x) GPRx(ARM_IR_R(_x))

#define rR_NAME(_x) arm_reg_name_lcase_string[0][ARM_IR_R(_x)]

#define SPR64x(_x) pARM_VM->spr64[_x]
#define rSPR64(_x) SPR64x(ARM_VM_SPR64(_x))
#define rSPR64hi(_x) ((rSPR64(_x) >> 32) & 0xffff)
#define rSPR64lo(_x) (rSPR64(_x) & 0xffff)

#define CPSR rSPR32(CPSR)
#define pSPSR pARM_VM->spsr
#define SPSR (pSPSR ? *pSPSR : 0)

#define CYCLE rSPR64(CYCLE)
#define ICOUNT rSPR64(ICOUNT)

#define IP rSPR32(IP)
#define IR rSPR32(IR)

/* **** */
