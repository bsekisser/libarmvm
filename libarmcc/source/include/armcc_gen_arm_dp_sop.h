#pragma once

/* **** */

#include "libarm/include/sop.h"

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#include <stdint.h>

/* **** */

static armcc_sop_t gen_arm_dp_sop__rm_i(gpr_eref rm, arm_sop_eref shift_type, const int rs)
{ return(pbBFMOV(rs, 0, 5, 7) | gen_arm_ir__shift_type(shift_type) | gen_arm_ir__rm(rm)); }

static armcc_sop_t gen_arm_dp_sop__rm_rs(gpr_eref rm, arm_sop_eref shift_type, gpr_eref rs)
{ return(gen_arm_ir__rs(rs) | gen_arm_ir__shift_type(shift_type) | (1 << 4) | gen_arm_ir__rm(rm)); }

static armcc_sop_t gen_arm_dp_sop__ror_i(const int imm, const int rs)
{ return(_BV(25) | (imm & 0xff) | pbBFMOV(rs, 1, 4, 8)); }
