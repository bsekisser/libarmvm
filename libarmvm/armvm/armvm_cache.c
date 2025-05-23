#include "armvm_cache.h"
#include "armvm_core.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"

/* **** */

static void __armvm_cache_alloc_init(armvm_cache_ref acr)
{
	ACTION_LOG(alloc_init);
	ERR_NULL(acr);

	acr->core = acr->armvm->core;
}

static void __armvm_cache_init(armvm_cache_ref acr)
{
	ACTION_LOG(init);
	ERR_NULL(acr);

	armvm_cache_cp15_init(acr);
}

static void __armvm_cache_exit(armvm_cache_ref acr)
{
	ACTION_LOG(exit);

	handle_ptrfree(acr);
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

armvm_cache_ptr armvm_cache_alloc(armvm_ref avm,
	armvm_cache_href const h2c)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2c);
	ERR_NULL(avm);

	armvm_cache_ref acr =
		handle_calloc(h2c, 1, sizeof(armvm_cache_t));

	ERR_NULL(acr);

	/* **** */

	acr->armvm = avm;

	/* **** */

	return(acr);
}
