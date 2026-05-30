#pragma once

/* **** */

#include "libarm/include/sop.h"

/* **** */

#include <stdint.h>

/* **** */

WARN_UNUSED_RESULT
uint32_t arm_shiftbox(arm_sop_eref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in);

WARN_UNUSED_RESULT
uint32_t arm_shiftbox_c(arm_sop_eref shift_type, const uint32_t rm, const uint32_t rs);

WARN_UNUSED_RESULT
uint32_t arm_shiftbox_immediate(arm_sop_eref shift_type, const uint32_t rm, const uint32_t rs, const unsigned carry_in);

WARN_UNUSED_RESULT
uint32_t arm_shiftbox_c_immediate(arm_sop_eref shift_type, const uint32_t rm, const uint32_t rs);
