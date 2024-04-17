#pragma once

/* **** */

#include "libbse/include/unused.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

UNUSED_FN
static int armvm_core_mem_ifetch(armvm_core_p const core, uint32_t *const ir, const uint32_t ea, const size_t size)
{ return(armvm_mmu_ifetch(core->mmu, ir, ea, size)); }

UNUSED_FN
static int armvm_core_mem_read(armvm_core_p const core, uint32_t *const read, const uint32_t ea, const size_t size)
{ return(armvm_mmu_read(core->mmu, read, ea, size)); }

UNUSED_FN
static int armvm_core_mem_write(armvm_core_p const core, const uint32_t ea, const size_t size, const uint32_t write)
{ return(armvm_mmu_write(core->mmu, ea, size, write)); }
