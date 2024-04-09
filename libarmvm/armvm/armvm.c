#define pARMVM_CORE avm->core

#include "armvm_glue.h"
#include "armvm.h"

/* **** */

#include "armvm_action.h"
#include "armvm_config.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"

/* **** */

static void _armvm_alloc_init(armvm_p const avm)
{
	ERR_NULL(avm);

	/* **** */
}

static void _armvm_exit(armvm_p avm)
{
	handle_free((void**)avm->h2avm);
}

/* **** */

void armvm(unsigned const action, armvm_p const avm)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT:
			_armvm_alloc_init(avm);
	}
//
	armvm_coprocessor(action, avm->coprocessor);
	armvm_core(action, avm->core);
	armvm_mem(action, avm->mem);
	armvm_mmu(action, avm->mmu);
//
	switch(action) {
		case ARMVM_ACTION_EXIT:
			return(_armvm_exit(avm));
	}
}

armvm_p armvm_alloc(armvm_h const h2avm)
{
	ERR_NULL(h2avm);

	const armvm_p avm = handle_calloc((void*)h2avm, 1, sizeof(armvm_t));
	ERR_NULL(avm);

	avm->h2avm = h2avm;

	/* **** */

	armvm_coprocessor_alloc(&avm->coprocessor, avm);
	armvm_core_alloc(&avm->core, avm);
	armvm_mem_alloc(&avm->mem, avm);
	armvm_mmu_alloc(&avm->mmu, avm);

	/* **** */

	return(avm);
}

void armvm_alloc_init(armvm_p const avm)
{
	armvm(ARMVM_ACTION_ALLOC_INIT, avm);
	armvm(ARMVM_ACTION_INIT, avm);
}

void armvm_exit(armvm_p const avm)
{ armvm(ARMVM_ACTION_EXIT, avm); }

void armvm_reset(armvm_p const avm)
{ armvm(ARMVM_ACTION_RESET, avm); }

uint64_t armvm_run(const uint64_t run_cycles, armvm_p const avm)
{
	uint64_t run_cycles_left = run_cycles;

	for(;run_cycles_left;) {
		const uint64_t start_cycle = CYCLE;

		armvm_step(avm);

		const uint64_t delta_cycles = CYCLE - start_cycle;
		const uint64_t _cycles_left = run_cycles_left - delta_cycles;

		if(run_cycles_left < delta_cycles)
			return(_cycles_left);

		run_cycles_left = _cycles_left;
	}

	return(run_cycles_left);
}

void armvm_step(armvm_p const avm)
{ return(armvm_core_step(avm->core)); }
