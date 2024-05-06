#pragma once

/* **** */

#include "armvm_core_mem.h"
#include "armvm_core.h"

/* **** */

#include <stdint.h>

/* **** */

static int __ldm(armvm_core_p const core, const unsigned r, const uint32_t ea)
{ return(armvm_core_mem_read(core, &GPRx(r), ea, 4)); }

static int __ldm_pc(armvm_core_p const core, const uint32_t ea)
{
	uint32_t new_pc = 0;

	if(0 > armvm_core_mem_read(core, &new_pc, ea, 4))
		return(0);

	return(armvm_core_pcx_v5(core, new_pc));
}

static int __stm(armvm_core_p const core, const unsigned r, const uint32_t ea)
{ return(armvm_core_mem_write(core, ea, 4, GPRx(r))); }
