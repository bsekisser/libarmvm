#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

void _armvm_trace_(armvm_core_ref core, const char* format, ...);
void _armvm_trace_comment(armvm_core_ref core, const char* format, ...);
void _armvm_trace_end(armvm_core_ref core, const char* format, ...);
void _armvm_trace_end_with_comment(armvm_core_ref core, const char* format, ...);

__attribute__((warn_unused_result))
int _armvm_trace_start(armvm_core_ref core, const char* format, ...);

void _armvm_trace(armvm_core_ref core, const char* format, ...);
