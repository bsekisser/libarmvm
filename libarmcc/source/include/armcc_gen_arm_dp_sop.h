#pragma once

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#include <stdint.h>

/* **** */

static arm_sop_t gen_arm_dp_sop__ror_i(const uint8_t imm, const uint8_t rs)
{ return((imm & 0xff) | pbBFMOV(rs, 1, 4, 8)); }
