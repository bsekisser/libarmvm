#pragma once

/* **** */

#define pARM_TRACE atp
#define pARM_VM (pARM_TRACE->arm_vm)

#include "core_trace_glue.h"

/* **** */

#define rR_NAME(_x) arm_reg_name_lcase_string[0][rR(_x)]

#define IP vR(IP)
#define IR vR(IR)
