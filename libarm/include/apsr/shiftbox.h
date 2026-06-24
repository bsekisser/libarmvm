#pragma once

/* **** */

#include "apsr.h"
#include "sop.h"

/* **** */

#include <stdint.h>

/* **** */

uint32_t arm_apsr_shiftbox(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs);
uint32_t arm_apsr_shiftbox_immediate(arm_apsr_ref apsr, arm_sop_eref shift_type, const uint32_t rm, const unsigned rs);
