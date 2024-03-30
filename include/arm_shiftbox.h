#pragma once

/* **** */

#include "arm_sop.h"

/* **** */

#include <stdint.h>

/* **** */

uint32_t arm_shiftbox(unsigned shift_type, uint32_t rm, uint32_t rs, unsigned carry_in);
uint32_t arm_shiftbox_c(unsigned shift_type, uint32_t rm, uint32_t rs);
uint32_t arm_shiftbox_immediate(unsigned shift_type, uint32_t rm, uint32_t rs, unsigned carry_in);
uint32_t arm_shiftbox_c_immediate(unsigned shift_type, uint32_t rm, uint32_t rs);
