#pragma once

/* **** */

#include "arm_sop.h"

/* **** */

#define arm_cc_string arm_cc_lcase_string
extern const char* arm_cc_lcase_string[2][16];

extern const char* arm_cc_ucase_string[2][16];
extern const char* arm_creg_name_string[16];
extern const char* arm_dp_inst_string[16];
extern const char* arm_dp_op_string[16];
extern const char* arm_reg_name_lcase_string[2][16];
extern const char* arm_sop_lcase_string[_ARM_SOP_COUNT_];
extern const char* arm_sop_ucase_string[_ARM_SOP_COUNT_];
