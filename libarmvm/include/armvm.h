#pragma once

/* **** */

typedef struct armvm_tag** armvm_h;
typedef armvm_h const armvm_href;

typedef struct armvm_tag* armvm_p;
typedef armvm_p const armvm_ref;

/* **** */

#include "armvm_cache.h"
#include "armvm_config.h"
#include "armvm_coprocessor.h"
#include "armvm_core.h"
#include "armvm_mem.h"
#include "armvm_mmu.h"
#include "armvm_trace.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

#include "stdint.h"
#include "stdlib.h"

/* **** */

typedef struct armvm_tag {
	armvm_coprocessor_ptr coprocessor;
	armvm_cache_ptr cache;
	armvm_core_p core;
	armvm_mem_p mem;
	armvm_mmu_ptr mmu;
//
	armvm_config_t config;
	armvm_h h2avm;
}armvm_t;

/* **** */

void armvm(armvm_p const avm, action_ref action);
armvm_p armvm_alloc(armvm_h const h2avm);
void armvm_alloc_init(armvm_p const avm);
void armvm_exit(armvm_p const avm);
uint32_t armvm_gpr(armvm_p const avm, const unsigned r, uint32_t *const write);
uint32_t* armvm_p2gpr(armvm_p const avm, const unsigned r);
void armvm_reset(armvm_p const avm);
uint64_t armvm_run(armvm_p const avm, uint64_t cycles);
uint32_t armvm_spr32(armvm_p const avm, const unsigned r);
uint64_t armvm_spr64(armvm_p const avm, const unsigned r);
int armvm_step(armvm_p const avm);
