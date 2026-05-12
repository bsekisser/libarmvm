#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

void armvm_core_exception(armvm_core_ref core, action_ref action);

//__attribute__((warn_unused_result))
int armvm_core_exception_data_abort(armvm_core_ref core);

//__attribute__((warn_unused_result))
int armvm_core_exception_prefetch_abort(armvm_core_ref core);

void armvm_core_exception_reset(armvm_core_ref core);

//__attribute__((warn_unused_result))
int armvm_core_exception_swi(armvm_core_ref core);

//__attribute__((warn_unused_result))
int armvm_core_exception_undefined_instruction(armvm_core_ref core);
