#pragma once

/* **** */

typedef struct armvm_tag** armvm_hptr;
typedef armvm_hptr const armvm_href;

typedef struct armvm_tag* armvm_ptr;
typedef armvm_ptr const armvm_ref;

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

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

/* **** */

typedef struct armvm_tag {
	armvm_coprocessor_ptr coprocessor;
	armvm_cache_ptr cache;
	armvm_core_ptr core;
	armvm_mem_ptr mem;
	armvm_mmu_ptr mmu;
//
	armvm_config_t config;
	pthread_t thread;
}armvm_t;

/* **** */

void armvm(armvm_ref avm, action_ref action);
armvm_ptr armvm_alloc(armvm_href h2avm);
void armvm_alloc_init(armvm_ref avm);
void armvm_exit(armvm_ref avm);
uint32_t armvm_gpr(armvm_ref avm, const unsigned r, uint32_t *const write);
uint32_t* armvm_p2gpr(armvm_ref avm, const unsigned r);
void armvm_reset(armvm_ref avm);
uint64_t armvm_run(armvm_ref avm, uint64_t cycles);
uint32_t armvm_spr32(armvm_ref avm, const unsigned r);
uint64_t armvm_spr64(armvm_ref avm, const unsigned r);
int armvm_step(armvm_ref avm);
int armvm_threaded_start(armvm_ref avm);
