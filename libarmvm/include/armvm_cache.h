#pragma once

/* **** */

typedef struct armvm_cache_tag* armvm_cache_p;
typedef armvm_cache_p const armvm_cache_ref;

typedef struct armvm_cache_tag** armvm_cache_h;
typedef armvm_cache_h const armvm_cache_href;

/* **** */

#include "armvm.h"
#include "armvm_core.h"

/* **** */

typedef struct armvm_cache_tag {
	armvm_p armvm;
	armvm_core_p core;
	armvm_cache_h h2c;
}armvm_cache_t;

/* **** */

void armvm_cache(armvm_cache_ref acr, const unsigned action);
armvm_cache_p armvm_cache_alloc(armvm_ref avm,
	armvm_cache_href const h2c);

void armvm_cache_cp15_init(armvm_cache_ref acr);
