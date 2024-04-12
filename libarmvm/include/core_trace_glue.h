#pragma once

/* **** */

#include "armvm_glue.h"

/* **** */

#define rR_IS_NOT_PC(_x) (ARMVM_GPR(PC) != ARM_IR_R(_x))
#define rR_IS_PC(_x) (ARMVM_GPR(PC) == ARM_IR_R(_x))

#define rR_NAMEx(_x) arm_reg_name_lcase_string[0][_x]

#define rRx(_x) pARMVM_TRACE->rr[_x]
#define rR(_x) pARMVM_TRACE->rr[ARMVM_TRACE_R(_x)]

#define rR64x(_x) pARMVM_TRACE->rr64[_x]
#define rR64(_x) pARMVM_TRACE->rr64[ARMVM_TRACE_R(_x)]

#define vRx(_x) pARMVM_TRACE->vr[_x]
#define vR(_x) pARMVM_TRACE->vr[ARMVM_TRACE_R(_x)]

#define vR64x(_x) pARMVM_TRACE->vr64[_x]
#define vR64(_x) pARMVM_TRACE->vr64[ARMVM_TRACE_R(_x)]
