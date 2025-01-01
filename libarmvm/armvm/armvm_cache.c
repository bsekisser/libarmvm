#include "armvm_action.h"
#include "armvm_cache.h"
#include "armvm_core.h"

/* **** */

#include "git/libbse/include/handle.h"
#include "git/libbse/include/log.h"

/* **** */

static void __armvm_cache_alloc_init(armvm_cache_ref acr)
{
	if(acr->armvm->config.trace.alloc_init) LOG();

	acr->core = acr->armvm->core;
}

static void __armvm_cache_init(armvm_cache_ref acr)
{
	armvm_cache_cp15_init(acr);
}

static void __armvm_cache_exit(armvm_cache_ref acr)
{
	if(acr->armvm->config.trace.exit) LOG();

	handle_free((void*)acr->h2c);
}

/* **** */

void armvm_cache(armvm_cache_ref acr, const unsigned action)
{
	ERR_NULL(acr);

	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: __armvm_cache_alloc_init(acr); break;
		case ARMVM_ACTION_INIT: __armvm_cache_init(acr); break;
	}
//
//
	switch(action) {
		case ARMVM_ACTION_EXIT: __armvm_cache_exit(acr); break;
	}
}

armvm_cache_p armvm_cache_alloc(armvm_p const avm,
	armvm_cache_href const h2c)
{
	ERR_NULL(h2c);
	ERR_NULL(avm);

	if(avm->config.trace.alloc) LOG();

	armvm_cache_ref acr =
		handle_calloc((void*)h2c, 1, sizeof(armvm_cache_t));

	ERR_NULL(acr);

	/* **** */

	acr->armvm = avm;
	acr->h2c = h2c;

	/* **** */

	return(acr);
}
