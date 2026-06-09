#pragma once

/* **** */

typedef struct libarmvm_tag** libarmvm_hptr;
typedef libarmvm_hptr const libarmvm_href;

typedef struct libarmvm_tag* libarmvm_ptr;
typedef libarmvm_ptr const libarmvm_ref;

/* **** */

#include "armvm_cache.h"
#include "armvm_config.h"
#include "armvm_coprocessor.h"
#include "armvm_core.h"
#include "armvm_mem.h"
#include "armvm_mmu.h"

#include "libarmvm_state.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

/* **** */

typedef struct hit_miss_tag {
	unsigned hit;
	unsigned miss;
}hit_miss_t;

typedef struct libarmvm_tag {
	armvm_coprocessor_ptr coprocessor;
	armvm_cache_ptr cache;
	armvm_core_ptr core;
	armvm_mem_ptr mem;
	armvm_mmu_ptr mmu;
//
	armvm_config_t config;
	pthread_t thread;
//
	volatile libarmvm_state_t state;
#define _STATE avm->state
#define STATE(_) (_STATE._)
//
	struct {
		unsigned hot[256][2];
		struct tlb {
			hit_miss_t ifetch;
			hit_miss_t read;
			hit_miss_t write;
		}tlb;
	}stats;
}libarmvm_t;

#define HOT 1
#ifndef HOT
	#define HOT 0
#endif

#define STATS 1
#ifndef STATS
	#define STATS 0
#endif

/* **** */


////__attribute__((warn_unused_result))
//uint32_t armvm_gpr(libarmvm_ref avm, const unsigned r, uint32_t *const write);

//__attribute__((warn_unused_result))
//uint32_t* armvm_p2gpr(libarmvm_ref avm, const unsigned r);

//void armvm_reset(libarmvm_ref avm);

//__attribute__((warn_unused_result))
//uint32_t armvm_spr32(libarmvm_ref avm, const unsigned r);

//__attribute__((warn_unused_result))
//uint64_t armvm_spr64(libarmvm_ref avm, const unsigned r);
