#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

void armvm_core_exception(unsigned action, armvm_core_p core);
void armvm_core_exception_data_abort(armvm_core_p core);
void armvm_core_exception_prefetch_abort(armvm_core_p core);
void armvm_core_exception_reset(armvm_core_p core);
void armvm_core_exception_swi(armvm_core_p core);
void armvm_core_exception_undefined_instruction(armvm_core_p core);
