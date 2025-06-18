#define pARMVM_CORE avm->core

#include "armvm_glue.h"
#include "armvm.h"

/* **** */

#include "armvm_config.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"
#include "libbse/include/mem_access.h"

/* **** */

#include <pthread.h>

/* **** */

static
int armvm_action_exit(int err, void *const param, action_ref)
{
	ACTION_LOG(exit);

	/* **** */

	handle_ptrfree(param);

	/* **** */

	return(err);
}

void armvm(armvm_ref avm, action_ref action)
{ action_handler(0, avm, action, &armvm_action_list); }

armvm_ptr armvm_alloc(armvm_href h2avm)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2avm);

	armvm_ref avm = handle_calloc(h2avm, 1, sizeof(armvm_t));
	ERR_NULL(avm);

	/* **** */

	armvm_cache_alloc(avm, &avm->cache);
	armvm_coprocessor_alloc(avm, &avm->coprocessor);
	armvm_core_alloc(avm, &avm->core);
	armvm_mem_alloc(avm, &avm->mem);
	armvm_mmu_alloc(avm, &avm->mmu);

	/* **** */

	return(avm);
}

void armvm_alloc_init(armvm_ref avm)
{
	armvm(avm, _ACTION_ALLOC_INIT);
	armvm(avm, _ACTION_INIT);
}

void armvm_exit(armvm_ref avm)
{ armvm(avm, _ACTION_EXIT); }

uint32_t armvm_gpr(armvm_ref avm, const unsigned r, uint32_t *const write)
{
	assert(_ARMVM_GPR_COUNT_ > r);
	return(mem_32_access(&GPRx(r), write));
}

uint32_t* armvm_p2gpr(armvm_ref avm, const unsigned r)
{
	assert(_ARMVM_GPR_COUNT_ > r);
	return(&GPRx(r));
}

void armvm_reset(armvm_ref avm)
{
	ACTION_LOG(reset);

	armvm(avm, _ACTION_RESET);
}

uint64_t armvm_run(armvm_ref avm, const uint64_t run_cycles)
{
	armvm_core_ref core = avm->core;

	uint64_t run_cycles_left = run_cycles;

	for(;run_cycles_left;) {
		if(core->flags.halt || core->flags.crashed)
			return(0);

		const uint64_t start_cycle = CYCLE;

		if(0 > armvm_step(avm))
			return(0);

		const uint64_t delta_cycles = CYCLE - start_cycle;
		const uint64_t _cycles_left = run_cycles_left - delta_cycles;

		if(run_cycles_left < delta_cycles)
			return(_cycles_left);

		run_cycles_left = _cycles_left;
	}

	return(run_cycles_left);
}

uint32_t armvm_spr32(armvm_ref avm, const unsigned r)
{
	assert(_ARMVM_SPR32_COUNT_ > r);
	return(SPR32x(r));
}

uint64_t armvm_spr64(armvm_ref avm, const unsigned r)
{
	assert(_ARMVM_SPR64_COUNT_ > r);
	return(SPR64x(r));
}

int armvm_step(armvm_ref avm)
{
	armvm_core_ref core = avm->core;

	if(core->flags.halt)
		return(-1);

	if(core->flags.crashed)
		return(0);

	const int rval = armvm_core_step(core);
		core->flags.halt |= (0 < rval);
		core->flags.crashed |= (rSPR32(IP) == PC);

	return(rval);
}

void* armvm_threaded_run(void* param)
{ return(armvm_core_threaded_run(((armvm_ref)param)->core)); }

int armvm_threaded_start(armvm_ref avm)
{ return(pthread_create(&avm->thread, 0, armvm_threaded_run, avm)); }

static
action_handler_t armvm_action_sublist[] = {
	{{ .list = &armvm_cache_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(armvm_t, cache) },
	{{ .list = &armvm_coprocessor_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(armvm_t, coprocessor) },
	{{ .list = &armvm_core_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(armvm_t, core) },
	{{ .list = &armvm_mem_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(armvm_t, mem) },
	{{ .list = &armvm_mmu_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(armvm_t, mmu) },
};

ACTION_LIST(armvm_action_list,
	.list = {
//		[_ACTION_ALLOC] = {{ armvm_action_alloc }, { 0 }, 0 },
		[_ACTION_EXIT] = {{ armvm_action_exit }, { 0 }, 0 },
	},

	SUBLIST(armvm_action_sublist),
);
