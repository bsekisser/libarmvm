#pragma once

/* **** */

typedef struct armvm_trace_t** armvm_trace_h;
typedef struct armvm_trace_t* armvm_trace_p;

#include "armvm_core.h"
//#include "armvm.h"

/* **** */

#include <stdint.h>

/* **** */

#define ARMVM_TRACE_R(_x) ARMVM_TRACE_R##_x
#define ARMVM_TRACE_R64(_x) ARMVM_TRACE_R##_x

enum _ARMVM_TRACE_R32 {
	ARMVM_TRACE_RD,
	ARMVM_TRACE_RM,
	ARMVM_TRACE_RN,
	ARMVM_TRACE_RS,
//
	ARMVM_TRACE_RDHi,
	ARMVM_TRACE_REA,
	ARMVM_TRACE_RIP,
	ARMVM_TRACE_RIR,
	ARMVM_TRACE_RSOP,
//
	_ARMVM_TRACE_R_COUNT_,
//
	ARMVM_TRACE_RDLo = ARMVM_TRACE_RD,
};

enum _ARMVM_TRACE_R64 {
	ARMVM_TRACE_R64D,
//
	_ARMVM_TRACE_R64_COUNT_
};

typedef struct armvm_trace_t {
//	uint64_t vr64[_ARMVM_TRACE_R64_COUNT_];
	uint32_t vr[_ARMVM_TRACE_R_COUNT_];

//	unsigned rr64[_ARMVM_TRACE_R64_COUNT_];
	unsigned rr[_ARMVM_TRACE_R_COUNT_];
//
	armvm_core_p armvm_core;
}armvm_trace_t;

/* **** */

void armvm_trace(armvm_trace_p atp);
