#pragma once

/* **** */

#include "apsr/apsr.h"
#include "dp.h"
#include "sop.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
uint32_t arm_apsr_alubox(arm_apsr_ref apsr, arm_dp_opcode_eref opcode, const uint32_t rn, const uint32_t sop);
