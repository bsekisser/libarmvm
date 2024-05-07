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
#include "libbse/include/mem_access.h"

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

void armvm(armvm_p const avm, unsigned const action)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT:
			_armvm_alloc_init(avm);
	}
//
	armvm_coprocessor(avm->coprocessor, action);
	armvm_core(avm->core, action);
	armvm_mem(avm->mem, action);
	armvm_mmu(avm->mmu, action);
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

	armvm_coprocessor_alloc(avm, &avm->coprocessor);
	armvm_core_alloc(avm, &avm->core);
	armvm_mem_alloc(avm, &avm->mem);
	armvm_mmu_alloc(avm, &avm->mmu);

	/* **** */

	return(avm);
}

void armvm_alloc_init(armvm_p const avm)
{
	armvm(avm, ARMVM_ACTION_ALLOC_INIT);
	armvm(avm, ARMVM_ACTION_INIT);
}

void armvm_exit(armvm_p const avm)
{ armvm(avm, ARMVM_ACTION_EXIT); }

uint32_t armvm_gpr(armvm_p const avm, const unsigned r, uint32_t *const write)
{
	assert(_ARMVM_GPR_COUNT_ > r);
	return(mem_32_access(&GPRx(r), write));
}

void armvm_reset(armvm_p const avm)
{
//	if(avm->config.trace.reset) LOG();

	armvm(avm, ARMVM_ACTION_RESET);
}

uint64_t armvm_run(armvm_p const avm, const uint64_t run_cycles)
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

uint32_t armvm_spr32(armvm_p const avm, const unsigned r)
{
	assert(_ARMVM_SPR32_COUNT_ > r);
	return(SPR32x(r));
}

uint64_t armvm_spr64(armvm_p const avm, const unsigned r)
{
	assert(_ARMVM_SPR64_COUNT_ > r);
	return(SPR64x(r));
}

int armvm_step(armvm_p const avm)
{ return(armvm_core_step(avm->core)); }
