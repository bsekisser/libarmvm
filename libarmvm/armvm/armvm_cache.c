#include "armvm_cache.h"
#include "armvm_core.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"

/* **** */

static
int armvm_cache_action_alloc_init(int err, void *const param, action_ref)
{
	ACTION_LOG(alloc_init, "err: 0x%08x, param: 0x%016" PRIxPTR, err, (uintptr_t)param);
	ERR_NULL(param);

	armvm_cache_ref acr = param;

	/* **** */

	ERR_NULL(acr->armvm);
	ERR_NULL(acr->core = acr->armvm->core);
	ERR_NULL(acr->cp = acr->armvm->coprocessor);

	/* **** */

	return(err);
}

static
int armvm_cache_action_exit(int err, void *const param, action_ref)
{
	ACTION_LOG(exit);

	/* **** */

	handle_ptrfree(param);

	/* **** */

	return(err);
}

armvm_cache_ptr armvm_cache_alloc(armvm_ref avm,
	armvm_cache_href const h2c)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2c);
	ERR_NULL(avm);

	armvm_cache_ref acr = handle_calloc(h2c, 1, sizeof(armvm_cache_t));
	ERR_NULL(acr);

	/* **** */

	acr->armvm = avm;

	/* **** */

	return(acr);
}

static
action_handler_t armvm_cache_action_sublist[] = {
	{{ .list = &armvm_cache_cp15_action_list }, { .is_list = 1 }, 0 },
	{{ 0 }, { 0 }, 0 },
};

ACTION_LIST(armvm_cache_action_list,
	.list = {
		[_ACTION_ALLOC_INIT] = {{ armvm_cache_action_alloc_init }, { 0 }, 0 },
		[_ACTION_EXIT] = {{ armvm_cache_action_exit }, { 0 }, 0 },
	},

	SUBLIST(armvm_cache_action_sublist),
);
