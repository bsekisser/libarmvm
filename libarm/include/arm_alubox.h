#pragma once

/* **** */

#include "arm_dp.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_alubox(arm_dp_opcode_tref operation, uint32_t *const rd, const uint32_t rn, const uint32_t sop, const unsigned carry_in);
