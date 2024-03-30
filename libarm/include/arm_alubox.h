#pragma once

/* **** */

#include "arm_dp.h"

/* **** */

#include <stdint.h>

/* **** */

uint32_t arm_alubox(unsigned operation, uint32_t* rd, uint32_t rn, uint32_t sop, unsigned carry_in);
