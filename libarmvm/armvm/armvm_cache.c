#include "armvm_cache.h"
#include "armvm_core.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"

/* **** */

static void __armvm_cache_alloc_init(armvm_cache_ref acr)
{
	if(action_log.at.alloc_init) LOG();

	acr->core = acr->armvm->core;
}

static void __armvm_cache_init(armvm_cache_ref acr)
{
	if(action_log.at.init) LOG();

	armvm_cache_cp15_init(acr);
}

static void __armvm_cache_exit(armvm_cache_ref acr)
{
	if(action_log.at.exit) LOG();

	handle_free((void*)acr->h2c);
}

/* **** */

void armvm_cache(armvm_cache_ref acr, action_ref action)
{
	ERR_NULL(acr);

	switch(action) {
		case _ACTION_ALLOC_INIT: __armvm_cache_alloc_init(acr); break;
		case _ACTION_INIT: __armvm_cache_init(acr); break;
		default: break;
	}
//
//
	switch(action) {
		case _ACTION_EXIT: __armvm_cache_exit(acr); break;
		default: break;
	}
}

armvm_cache_p armvm_cache_alloc(armvm_p const avm,
	armvm_cache_href const h2c)
{
	if(action_log.at.alloc) LOG();

	ERR_NULL(h2c);
	ERR_NULL(avm);

	armvm_cache_ref acr =
		handle_calloc((void*)h2c, 1, sizeof(armvm_cache_t));

	ERR_NULL(acr);

	/* **** */

	acr->armvm = avm;
	acr->h2c = h2c;

	/* **** */

	return(acr);
}
