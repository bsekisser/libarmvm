#pragma once

/* **** */

#include "libarm/include/arm_disasm.h"
#include "armvm.h"

/* **** */

//__attribute__((warn_unused_result))
int armvm_exception_fiq(armvm_ref avm);

//__attribute__((warn_unused_result))
int armvm_exception_irq(armvm_ref avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
