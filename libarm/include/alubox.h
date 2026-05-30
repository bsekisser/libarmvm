#pragma once

/* **** */

#include "libarm/include/dp.h"

/* **** */

#include <stdint.h>

/* **** */

WARN_UNUSED_RESULT
uint32_t arm_alubox(arm_dp_opcode_eref operation, uint32_t *const rd, const uint32_t rn, const uint32_t sop, const unsigned carry_in);
