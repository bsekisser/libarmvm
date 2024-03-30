#pragma once

/* **** */

#include "arm_trace.h"

/* **** */

void __trace_end(arm_trace_p atp);
int __trace_start(arm_trace_p atp);
void _arm_trace_(arm_trace_p atp, const char* format, ...);
void _arm_trace_comment(arm_trace_p atp, const char* format, ...);
void _arm_trace_end(arm_trace_p atp, const char* format, ...);
void _arm_trace_start(arm_trace_p atp, const char* format, ...);
void _arm_trace(arm_trace_p atp, const char* format, ...);
