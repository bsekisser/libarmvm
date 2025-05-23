#pragma once

/* **** */

typedef struct armvm_cache_tag** armvm_cache_hptr;
typedef armvm_cache_hptr const armvm_cache_href;

typedef struct armvm_cache_tag* armvm_cache_ptr;
typedef armvm_cache_ptr const armvm_cache_ref;

/* **** */

#include "armvm_coprocessor.h"
#include "armvm_core.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

typedef struct armvm_cache_tag {
	armvm_ptr armvm;
	armvm_core_ptr core;
	armvm_coprocessor_ptr cp;
}armvm_cache_t;

/* **** */

extern action_list_t armvm_cache_action_list;
extern action_list_t armvm_cache_cp15_action_list;

armvm_cache_ptr armvm_cache_alloc(armvm_ref avm,
	armvm_cache_href const h2c);
