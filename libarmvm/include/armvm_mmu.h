#pragma once

/* **** */

typedef struct armvm_mmu_t** armvm_mmu_h;
typedef struct armvm_mmu_t* armvm_mmu_p;

/* **** */

#include "armvm.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

void armvm_mmu(armvm_mmu_p const mem, action_ref action);
armvm_mmu_p armvm_mmu_alloc(armvm_p const avm, armvm_mmu_h const h2mem);
int armvm_mmu_ifetch(armvm_mmu_p const mem, uint32_t *const ir, const uint32_t va, const size_t size);
int armvm_mmu_read(armvm_mmu_p const mem, uint32_t *const read, const uint32_t va, const size_t size);
int armvm_mmu_write(armvm_mmu_p const mem, const uint32_t va, const size_t size, const uint32_t write);
