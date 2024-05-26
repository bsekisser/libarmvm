#pragma once

/* **** */

#include "libarm/include/arm_sop.h"

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#include <stdint.h>

/* **** */

static armcc_sop_t gen_arm_dp_sop__rm_i(const arm_reg_t rm, const arm_sop_t shift_type, const int rs)
{ return(pbBFMOV(rs, 0, 5, 7) | gen_arm_ir__shift_type(shift_type) | gen_arm_ir__rm(rm)); }

static armcc_sop_t gen_arm_dp_sop__ror_i(const int imm, const int rs)
{ return((imm & 0xff) | pbBFMOV(rs, 1, 4, 8)); }
