#pragma once

/* **** */

typedef struct armvm_mmu_tag** armvm_mmu_hptr;
typedef armvm_mmu_hptr const armvm_mmu_href;

typedef struct armvm_mmu_tag* armvm_mmu_ptr;
typedef armvm_mmu_ptr const armvm_mmu_ref;

/* **** */

#include "armvm.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

void armvm_mmu(armvm_mmu_ref mem, action_ref action);
armvm_mmu_ptr armvm_mmu_alloc(armvm_p const avm, armvm_mmu_href h2mem);
int armvm_mmu_ifetch(armvm_mmu_ref mem, uint32_t *const ir, const uint32_t va, const size_t size);
int armvm_mmu_read(armvm_mmu_ref mem, uint32_t *const read, const uint32_t va, const size_t size);
int armvm_mmu_write(armvm_mmu_ref mem, const uint32_t va, const size_t size, const uint32_t write);
