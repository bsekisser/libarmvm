#pragma once

/* **** */

enum {
	ARMVM_EXCEPTION_Reset,
	ARMVM_EXCEPTION_DataAbort,
	ARMVM_EXCEPTION_FIQ,
	ARMVM_EXCEPTION_IRQ,
	ARMVM_EXCEPTION_PrefetchAbort,
	ARMVM_EXCEPTION_SWI,
	ARMVM_EXCEPTION_UNDEFINED,
};

/* **** */

#include "libarm/include/arm_disasm.h"
#include "armvm.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

void arm_exception(unsigned exception, armvm_p avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
