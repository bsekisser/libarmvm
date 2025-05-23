#pragma once

/* **** */

typedef struct armvm_mmu_tag** armvm_mmu_hptr;
typedef armvm_mmu_hptr const armvm_mmu_href;

typedef struct armvm_mmu_tag* armvm_mmu_ptr;
typedef armvm_mmu_ptr const armvm_mmu_ref;

/* **** */

#include "armvm_coprocessor.h"
#include "armvm_core.h"
#include "armvm_mem.h"
#include "armvm_tlb.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/bitops.h"
#include "libbse/include/page.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

#define TTBCR(_write) rCP15x(cp15(0, 2, 0, 2), _write)
#define TTBCR_N mlBFEXT(TTBCR(0), 2, 0)

#define TTBR0(_write) rCP15x(cp15(0, 2, 0, 0), _write)
#define TTBR1(_write) rCP15x(cp15(0, 2, 0, 1), _write)

/* **** */

typedef struct armvm_mmu_tag {
	armvm_mem_callback_ptr l1[PAGE_SIZE];
//
	armvm_ptr armvm;

	armvm_core_ptr core;

	armvm_coprocessor_ptr cp;
	armvm_mem_ptr mem;
	armvm_tlb_ptr tlb;
}armvm_mmu_t;

/* **** */

void armvm_mmu(armvm_mmu_ref mem, action_ref action);
armvm_mmu_ptr armvm_mmu_alloc(armvm_ref avm, armvm_mmu_href h2mem);
int armvm_mmu_ifetch(armvm_mmu_ref mem, uint32_t *const ir, const uint32_t va, const size_t size);
int armvm_mmu_read(armvm_mmu_ref mem, uint32_t *const read, const uint32_t va, const size_t size);
int armvm_mmu_write(armvm_mmu_ref mem, const uint32_t va, const size_t size, const uint32_t write);

void armvm_mmu_cp15_init(armvm_mmu_ref mem);
