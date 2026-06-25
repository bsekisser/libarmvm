#pragma once

/* **** */

#include "libarm/include/apsr/apsr.h"
#include "libarm/include/dp.h"
#include "libarm/include/sop.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_apsr_alubox(arm_apsr_ref apsr, arm_dp_opcode_eref opcode, const uint32_t rn, const uint32_t sop);

uint32_t arm_apsr_flags_nz(arm_apsr_ref apsr, const uint32_t rd);

uint32_t arm_apsr_flags_nzc(arm_apsr_ref apsr, const uint32_t rd);
