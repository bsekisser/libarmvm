#pragma once

/* **** */

#include "armvm_core_glue.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_cc.h"
#include "libarm/include/arm_cpsr.h"

/* **** */

static int armvm_core_check_cc(armvm_core_ref core, const uint8_t cc)
{
	uint32_t res = 0;

	switch(cc & ~1U)
	{
		case CC_EQ_NE:
			res = IF_CPSR(Z);
			break;
		case CC_CS_CC:
			res = IF_CPSR(C);
			break;
		case CC_MI_PL:
			res = IF_CPSR(N);
			break;
		case CC_VS_VC:
			res = IF_CPSR(V);
			break;
		case CC_HI_LS:
			res = (IF_CPSR(C) && IF_NOT_CPSR(Z));
			break;
		case CC_GE_LT:
			res = (IF_CPSR(N) == IF_CPSR(V));
			break;
		case CC_GT_LE:
			res = (IF_NOT_CPSR(Z) && (IF_CPSR(N) == IF_CPSR(V)));
			break;
		case CC_AL_NV:
			res = 1;
			break;
		default:
			LOG("IR = 0x%08x, cc = %02x, cpsr = 0x%08x, cpsr_cc %02x",
				IR, cc, CPSR, mlBFEXT(CPSR, 31, 28));
			LOG_ACTION(exit(-1));
			break;
	}

	rSPR32(CC) = cc;

	res = !!res;
	res = (cc & 1) ? !res : res;

	CCX = res;

	return(res);
}
