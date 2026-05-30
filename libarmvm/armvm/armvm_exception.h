#pragma once

/* **** */

#include "libarm/include/disasm.h"
#include "armvm.h"

/* **** */

//__attribute__((warn_unused_result))
int armvm_exception_fiq(libarmvm_ref avm);

//__attribute__((warn_unused_result))
int armvm_exception_irq(libarmvm_ref avm);

/* **** */

#define UNDEFINED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }

#define UNIMPLIMENTED \
	{ arm_disasm(IP, IR); LOG_ACTION(exit(-1)); }
