#pragma once

/* **** */

#define pARMVM_TRACE pARMVM_CORE->armvm_trace
#define pARMVM_CORE core

#define CONFIG (&pARMVM_CORE->config)

#include "armvm_glue.h"
#include "core_trace_glue.h"

/* **** */

//#define irGPR(_x) GPRx(ARM_IR_R(_x))

#define irR_NAMEx(_x) rR_NAMEx(ARM_IR_R(_x))

#define IP rSPR32(IP)
#define IR rSPR32(IR)

/* **** */
