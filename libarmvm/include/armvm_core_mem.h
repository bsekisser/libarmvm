#pragma once

/* **** */
/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

static int armvm_core_mem_ifetch(armvm_core_p const core, uint32_t *const ir, const uint32_t ea, const size_t size)
{ return(armvm_mmu_ifetch(core->mmu, ir, ea, size)); }

static int armvm_core_mem_read(armvm_core_p const core, uint32_t *const read, const uint32_t ea, const size_t size)
{ return(armvm_mmu_read(core->mmu, read, ea, size)); }

static int armvm_core_mem_write(armvm_core_p const core, const uint32_t ea, const size_t size, const uint32_t write)
{ return(armvm_mmu_write(core->mmu, ea, size, write)); }
