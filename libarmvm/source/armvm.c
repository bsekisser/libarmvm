#define pCORE avm->core

#include "armvm.h"

/* **** */

#include "armvm_config.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/cpsr.h"

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
libarmvm_state_t _armvm_state(libarmvm_ref avm)
{
	armvm_core_ref core = avm->core;

	STATE(crashed) = core->flags.crashed;
	STATE(halt) = core->flags.halt;

	return(_STATE);
}

/* **** */

static WARN_UNUSED_RESULT
libarmvm_ptr _libarmvm_alloc(libarmvm_ref avm)
{
	ERR_NULL(avm);

	if(STATS)
		LOGzx32(sizeof(libarmvm_t));

	armvm_cache_alloc(avm, &avm->cache);
	armvm_coprocessor_alloc(avm, &avm->coprocessor);
	armvm_core_alloc(avm, &avm->core);
	armvm_mem_alloc(avm, &avm->mem);
	armvm_mmu_alloc(avm, &avm->mmu);

	return(avm);
}

/* **** */

static
int libarmvm_action_exit(int err, void *const param, action_ref)
{
	libarmvm_ref avm = param;

	ACTION_LOG(exit);

	/* **** */

	STATE(halt) = 1;
	STATE(running) = STATE(run) = 0;

	if(STATE(threaded)) {
		pthread_join(avm->thread, 0);
	}

	handle_ptrfree(param);

	/* **** */

	return(err);
}

static
action_handler_t libarmvm_action_sublist[] = {
	{{ .list = &armvm_cache_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(libarmvm_t, cache) },
	{{ .list = &armvm_coprocessor_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(libarmvm_t, coprocessor) },
	{{ .list = &armvm_core_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(libarmvm_t, core) },
	{{ .list = &armvm_mem_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(libarmvm_t, mem) },
	{{ .list = &armvm_mmu_action_list } , { .dereference = 1, .is_list = 1 }, offsetof(libarmvm_t, mmu) },
	{{ 0 }, { 0 }, 0 },
};

PUBLIC
ACTION_LIST(libarmvm_action_list,
	.list = {
//		[_ACTION_ALLOC] = {{ libarmvm_action_alloc }, { 0 }, 0 },
		[_ACTION_EXIT] = {{ libarmvm_action_exit }, { 0 }, 0 },
	},

	SUBLIST(libarmvm_action_sublist),
);

/* **** */

PUBLIC
void libarmvm(libarmvm_ref avm, action_ref action)
{ action_handler(0, avm, action, &libarmvm_action_list); }

PUBLIC WARN_UNUSED_RESULT
libarmvm_ptr libarmvm_alloc(void)
{
	ACTION_LOG(alloc);

	return(_libarmvm_alloc(calloc(1, sizeof(libarmvm_t))));
}

PUBLIC
void libarmvm_alloc_init(libarmvm_ref avm)
{
	libarmvm(avm, _ACTION_ALLOC_INIT);
	libarmvm(avm, _ACTION_INIT);
}

PUBLIC
void libarmvm_exit(libarmvm_ref avm)
{ libarmvm(avm, _ACTION_EXIT); }

PUBLIC
libarmvm_ptr libarmvm_halloc(libarmvm_href h2avm)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2avm);

	return(_libarmvm_alloc(handle_calloc(h2avm, 1, sizeof(libarmvm_t))));
}

PUBLIC
void libarmvm_hot(libarmvm_ref avm)
{
	for(unsigned x = 0; x < 256; x++)
	{
		unsigned group = (x >> 5) & 7;

		LOG_START();
		for(unsigned y = 0; y < 4; y++, x++) {
			unsigned opcode = (x >> 1) & 15;
			unsigned s = (x >> 1) & 1;

			uint32_t *const stat = &avm->stats.hot[x][0];

			_LOG_("[%01u:%02x:%01u](%08x, %08x), ",
				group, opcode, s, stat[0], stat[1]);
		}
		LOG_END();
	}
}

PUBLIC
uint64_t libarmvm_cycle(libarmvm_ref avm)
{ return(CYCLE); }

PUBLIC
uint64_t libarmvm_icount(libarmvm_ref avm)
{ return(ICOUNT); }

PUBLIC WARN_UNUSED_RESULT
uint32_t libarmvm_ip(libarmvm_ref avm)
{ return(IP); }

PUBLIC WARN_UNUSED_RESULT
uint32_t libarmvm_pc(libarmvm_ref avm)
{ return(PC); }

PUBLIC
void libarmvm_reset(libarmvm_ref avm)
{
	ACTION_LOG(reset);

	libarmvm(avm, _ACTION_RESET);
}

PUBLIC
uint64_t libarmvm_run(libarmvm_ref avm, const uint64_t run_cycles)
{
	armvm_core_ref core = avm->core;

	uint64_t run_cycles_left = run_cycles;

	for(;run_cycles_left;) {
		if(core->flags.halt || core->flags.crashed)
			return(0);

		const uint64_t start_cycle = CYCLE;

		libarmvm_step(avm);

		const uint64_t delta_cycles = CYCLE - start_cycle;
		const uint64_t _cycles_left = run_cycles_left - delta_cycles;

		if(run_cycles_left < delta_cycles)
			return(_cycles_left);

		run_cycles_left = _cycles_left;

		if(STATE(error)) break;
	}

	return(run_cycles_left);
}

PUBLIC
libarmvm_state_t libarmvm_state(libarmvm_ref avm)
{ return(_STATE); }

PUBLIC
void libarmvm_stats(libarmvm_ref avm)
{
	LOGu(avm->stats.tlb.ifetch.hit);
	LOGu(avm->stats.tlb.ifetch.miss);
	LOGu(avm->stats.tlb.read.hit);
	LOGu(avm->stats.tlb.read.miss);
	LOGu(avm->stats.tlb.write.hit);
	LOGu(avm->stats.tlb.write.miss);
}

PUBLIC // TODO: internal state struct -- istate
libarmvm_state_t libarmvm_step(libarmvm_ref avm)
{
	STATE(step) = 1;

 	armvm_core_ref core = avm->core;

	if(core->flags.halt)
		return(_armvm_state(avm));

	if(core->flags.crashed)
		return(_armvm_state(avm));

	const int rval = armvm_core_step(core);
		core->flags.halt |= (0 > rval);
		core->flags.crashed |= (IP == PC);

	STATE(step) = 0;

	return(_armvm_state(avm));
}

PUBLIC // TODO: should this be public???
void* libarmvm_threaded_run(void* param)
{ return(armvm_core_threaded_run(((libarmvm_ref)param)->core)); }

PUBLIC
int libarmvm_threaded_start(libarmvm_ref avm)
{ return(pthread_create(&avm->thread, 0, libarmvm_threaded_run, avm)); }

PUBLIC
unsigned libarmvm_trace(libarmvm_ref avm, int trace_set)
{
	const unsigned trace_out = avm->core->config.trace;

	if(trace_set)
		avm->core->config.trace = (0 < trace_set);

	return(trace_out);
}
