#pragma once

/* **** */

#include "armvm_core_mem.h"
#include "armvm_core.h"

/* **** */

#include <stdint.h>

/* **** */

static int core_ldm(armvm_core_p const core, const unsigned r, const uint32_t ea)
{ return(armvm_core_mem_read(core, &GPRx(r), ea, 4)); }

static int core_stm(armvm_core_p const core, const unsigned r, const uint32_t ea)
{ return(armvm_core_mem_write(core, ea, 4, GPRx(r))); }
