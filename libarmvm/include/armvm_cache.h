#pragma once

/* **** */

typedef struct armvm_cache_tag** armvm_cache_hptr;
typedef armvm_cache_hptr const armvm_cache_href;

typedef struct armvm_cache_tag* armvm_cache_ptr;
typedef armvm_cache_ptr const armvm_cache_ref;

/* **** */

#include "armvm.h"
#include "armvm_core.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

typedef struct armvm_cache_tag {
	armvm_ptr armvm;
	armvm_core_p core;
	armvm_cache_hptr h2c;
}armvm_cache_t;

/* **** */

void armvm_cache(armvm_cache_ref acr, action_ref action);
armvm_cache_ptr armvm_cache_alloc(armvm_ref avm,
	armvm_cache_href const h2c);

void armvm_cache_cp15_init(armvm_cache_ref acr);
