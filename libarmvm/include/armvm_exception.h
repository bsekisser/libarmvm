#pragma once

/* **** */

#include "libarm/include/arm_disasm.h"
#include "armvm.h"

/* **** */

void arm_exception(unsigned exception, armvm_ptr avm);
int armvm_exception_fiq(armvm_ref avm);
int armvm_exception_irq(armvm_ref avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
