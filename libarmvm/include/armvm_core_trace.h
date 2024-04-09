#pragma once

/* **** */

#include "armvm_trace.h"

/* **** */

void __trace_end(armvm_trace_p const atp);
int __trace_start(armvm_trace_p const atp);
void _armvm_trace_(armvm_trace_p const atp, const char* format, ...);
void _armvm_trace_comment(armvm_trace_p const atp, const char* format, ...);
void _armvm_trace_end(armvm_trace_p const atp, const char* format, ...);
void _armvm_trace_start(armvm_trace_p const atp, const char* format, ...);
void _armvm_trace(armvm_trace_p const atp, const char* format, ...);
