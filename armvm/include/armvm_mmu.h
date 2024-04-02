#pragma once

/* **** */

typedef struct armvm_mmu_t** armvm_mmu_h;
typedef struct armvm_mmu_t* armvm_mmu_p;

/* **** */

#include "armvm.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

void armvm_mmu(unsigned const action, armvm_mmu_p const mem);
armvm_mmu_p armvm_mmu_alloc(armvm_mmu_h h2mem, armvm_p const avm);
int armvm_mmu_ifetch(uint32_t *const ir, uint32_t const va, size_t const size, armvm_mmu_p const mmu);
int armvm_mmu_read(uint32_t *const read, uint32_t const va, size_t const size, armvm_mmu_p const mmu);
int armvm_mmu_write(uint32_t const va, size_t const size, uint32_t const write, armvm_mmu_p const mmu);
