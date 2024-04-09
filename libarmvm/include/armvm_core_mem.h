#pragma once

/* **** */
/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

static int armvm_core_mem_ifetch(uint32_t *const ir, uint32_t const ea, size_t const size, armvm_core_p const core)
{ return(armvm_mmu_ifetch(ir, ea, size, core->mmu)); }

static int armvm_core_mem_read(uint32_t *const read, uint32_t const ea, size_t const size, armvm_core_p const core)
{ return(armvm_mmu_read(read, ea, size, core->mmu)); }

static int armvm_core_mem_write(uint32_t const ea, size_t const size, uint32_t const write, armvm_core_p const core)
{ return(armvm_mmu_write(ea, size, write, core->mmu)); }
