#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

__attribute__((format(printf, 2, 3)))
void _itrace_(armvm_core_ref core, const char* format, ...);

__attribute__((format(printf, 2, 3)))
void _itrace_comment(armvm_core_ref core, const char* format, ...);

__attribute__((format(printf, 2, 3)))
void _itrace_end(armvm_core_ref core, const char* format, ...);

__attribute__((format(printf, 2, 3)))
void _itrace_end_with_comment(armvm_core_ref core, const char* format, ...);

__attribute__((format(printf, 2, 3)))
void itrace(armvm_core_ref core, const char* format, ...);

__attribute__((format(printf, 2, 3), warn_unused_result))
int itrace_start(armvm_core_ref core, const char* format, ...);
