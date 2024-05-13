#pragma once

/* **** */

#include "libarm/include/arm_disasm.h"
#include "armvm.h"

/* **** */

void arm_exception(unsigned exception, armvm_p avm);
int armvm_exception_fiq(armvm_p const avm);
int armvm_exception_irq(armvm_p const avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
