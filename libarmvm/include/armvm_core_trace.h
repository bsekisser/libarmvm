#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

void __trace_end(armvm_core_p const core);
int __trace_start(armvm_core_p const core);
void _armvm_trace_(armvm_core_p const core, const char* format, ...);
void _armvm_trace_comment(armvm_core_p const core, const char* format, ...);
void _armvm_trace_end(armvm_core_p const core, const char* format, ...);
int _armvm_trace_start(armvm_core_p const core, const char* format, ...);
void _armvm_trace(armvm_core_p const core, const char* format, ...);
