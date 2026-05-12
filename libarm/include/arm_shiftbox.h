#pragma once

/* **** */

#include "arm_sop.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_shiftbox(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in);

__attribute__((warn_unused_result))
uint32_t arm_shiftbox_c(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs);

__attribute__((warn_unused_result))
uint32_t arm_shiftbox_immediate(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in);

__attribute__((warn_unused_result))
uint32_t arm_shiftbox_c_immediate(arm_sop_tref shift_type, const uint32_t rm, const uint32_t rs);
