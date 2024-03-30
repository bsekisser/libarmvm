#pragma once

/* **** */

#define GPRx(_x) pARM_VM->gpr[_x]
#define SPR32x(_x) pARM_VM->spr32[_x]

#define rSPR32(_x) SPR32x(ARM_VM_SPR32(_x))

/* **** */

#define ARM_IP_NEXT ((4 + IP) & ~3)
#define ARM_PC_NEXT ((4 + ARM_IP_NEXT) & ~3)

#define THUMB_IP_NEXT ((2 + IP) & ~1)
#define THUMB_PC_NEXT ((2 + THUMB_IP_NEXT) & ~1)

#define rR_IS_NOT_PC(_x) (ARM_VM_GPR(PC) != ARM_IR_R(_x))
#define rR_IS_PC(_x) (ARM_VM_GPR(PC) == ARM_IR_R(_x))

#define rRx(_x) pARM_TRACE->rr[_x]
#define rR(_x) pARM_TRACE->rr[ARM_TRACE_R(_x)]

#define rR64x(_x) pARM_TRACE->rr64[_x]
#define rR64(_x) pARM_TRACE->rr64[ARM_TRACE_R(_x)]

#define vRx(_x) pARM_TRACE->vr[_x]
#define vR(_x) pARM_TRACE->vr[ARM_TRACE_R(_x)]

#define vR64x(_x) pARM_TRACE->vr64[_x]
#define vR64(_x) pARM_TRACE->vr64[ARM_TRACE_R(_x)]

#define vmGPR(_x) GPRx(ARM_VM_GPR(_x))
#define LR vmGPR(LR)
#define PC vmGPR(PC)

#define CCX rSPR32(CCX)
