#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

void _itrace_(armvm_core_ref core, const char* format, ...);
void _itrace_comment(armvm_core_ref core, const char* format, ...);
void _itrace_end(armvm_core_ref core, const char* format, ...);
void _itrace_end_with_comment(armvm_core_ref core, const char* format, ...);

__attribute__((warn_unused_result))
int _itrace_start(armvm_core_ref core, const char* format, ...);

void _armvm_trace(armvm_core_ref core, const char* format, ...);
