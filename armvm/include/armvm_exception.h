#pragma once

/* **** */

enum {
	ARM_VM_EXCEPTION_Reset,
	ARM_VM_EXCEPTION_DataAbort,
	ARM_VM_EXCEPTION_FIQ,
	ARM_VM_EXCEPTION_IRQ,
	ARM_VM_EXCEPTION_PrefetchAbort,
	ARM_VM_EXCEPTION_SWI,
	ARM_VM_EXCEPTION_UNDEFINED,
};

/* **** */

#include "arm_disasm.h"
#include "armvm.h"

/* **** */

#include "log.h"

/* **** */

void arm_exception(unsigned exception, armvm_p avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
