#pragma once

/* **** */

typedef struct arm_trace_t** arm_trace_h;
typedef struct arm_trace_t* arm_trace_p;

#include "armvm.h"

/* **** */

#include <stdint.h>

/* **** */

#define ARM_TRACE_R(_x) ARM_TRACE_R##_x
#define ARM_TRACE_R64(_x) ARM_TRACE_R##_x

enum _ARM_TRACE_R32 {
	ARM_TRACE_RD,
	ARM_TRACE_RM,
	ARM_TRACE_RN,
	ARM_TRACE_RS,
//
	ARM_TRACE_RDHi,
	ARM_TRACE_REA,
	ARM_TRACE_RIP,
	ARM_TRACE_RIR,
	ARM_TRACE_RSOP,
//
	_ARM_TRACE_R_COUNT_,
//
	ARM_TRACE_RDLo = ARM_TRACE_RD,
};

enum _ARM_TRACE_R64 {
	ARM_TRACE_R64D,
//
	_ARM_TRACE_R64_COUNT_
};

typedef struct arm_trace_t {
//	uint64_t vr64[_ARM_TRACE_R64_COUNT_];
	uint32_t vr[_ARM_TRACE_R_COUNT_];

//	unsigned rr64[_ARM_TRACE_R64_COUNT_];
	unsigned rr[_ARM_TRACE_R_COUNT_];
//
	armvm_p arm_vm;
}arm_trace_t;

/* **** */

void arm_trace(arm_trace_p atp);
