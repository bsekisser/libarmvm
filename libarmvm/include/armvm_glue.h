#pragma once

/* **** */

#define ARM_IP_NEXT ((4 + IP) & ~3)
#define ARM_PC_NEXT ((4 + ARM_IP_NEXT) & ~3)

#define CCX rSPR32(CCX)
#define CPSR rSPR32(CPSR)
#define CYCLE rSPR64(CYCLE)
#define ICOUNT rSPR64(ICOUNT)

#define THUMB_IP_NEXT ((2 + IP) & ~1)
#define THUMB_PC_NEXT ((2 + THUMB_IP_NEXT) & ~1)

#define LR vmGPR(LR)
#define PC vmGPR(PC)
#define SP vmGPR(SP)

#define rABT(_x) rSPR32(ABT_##_x)
#define rFIQ(_x) rSPR32(FIQ_##_x)
#define rIRQ(_x) rSPR32(IRQ_##_x)
#define rSVC(_x) rSPR32(SVC_##_x)
#define rUND(_x) rSPR32(UND_##_x)
