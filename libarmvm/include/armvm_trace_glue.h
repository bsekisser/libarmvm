#pragma once

/* **** */

#define pARMVM_TRACE atp
#define pARMVM_CORE (pARMVM_TRACE->armvm_core)

#include "core_trace_glue.h"

/* **** */

#define rR_NAME(_x) rR_NAMEx(rR(_x))

#define IP vR(IP)
#define IR vR(IR)
