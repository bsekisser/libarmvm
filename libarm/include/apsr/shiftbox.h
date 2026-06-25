#pragma once

/* **** */

#include "libarm/include/apsr/apsr.h"
#include "libarm/include/sop.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((warn_unused_result))
typedef uint32_t (*arm_apsr_shiftbox_fn)(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs);

__attribute__((warn_unused_result))
uint32_t arm_apsr_shiftbox(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs);

__attribute__((warn_unused_result))
uint32_t arm_apsr_shiftbox_immediate(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs);
