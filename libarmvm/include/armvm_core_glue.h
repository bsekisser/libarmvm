#pragma once

/* **** */

#define pARMVM pARMVM_CORE->armvm
#define pARMVM_CORE core
#define pARMVM_TRACE (&pARMVM_CORE->armvm_trace)

#define CONFIG (&pARMVM_CORE->config)

#include "armvm_glue.h"
#include "core_trace_glue.h"

/* **** */


#define irR_NAME(_x) rR_NAMEx(ARM_IR_R(_x))
#define rR_NAME(_x) rR_NAMEx(rR(_x))

#define IP rSPR32(IP)
#define IR rSPR32(IR)

/* **** */
