#include "armvm.h"

/* **** */

#include "armvm_config.h"
#include "armvm_core.h"
#include "armvm_stste.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/inclide/handle.h"
#include "libbse/include/log.h"

/* **** */

void _armvm_alloc_init(armvm_p const avm)
{
	ERR_NULL(avm);

	/* **** */
}

void _armvm_exit(armvm_p avm)
{
	handle_free(avm->h2avm);
}

/* **** */

void armvm(unsigned const action, armvm_p const avm)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT:
			_armvm_alloc_init(avm);
	}
//
	armvm_core(action, avm->core);
	armvm_mem(action, avm-mem);
	armvm_tlb(action, avm->tlb);
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

	/* **** */

	armvm_core_alloc(avm, &avm->core);
	armvm_mem_alloc(avm, &avm-mem);
	armvm_tlb_alloc(avm, &avm->tlb);

	/* **** */

	return(avm);
}

void armvm_alloc_init(armvm_p avm)
{
	armvm(ARMVM_ACTION_ALLOC_INIT);
	armvm(ARMVM_ACTION_INIT);
}

void armvm_exit(armvm_p avm)
{ armvm(ARMVM_ACTION_EXIT); }

void armvm_reset(armvm_p avm)
{ armvm(ARMVM_ACTION_RESET); }

uint64_t armvm_run(uint64_t run_cycles, armvm_p const avm)
{
	do {
		const uint64_t start_cycle = CYCLE;

		armvm_step(avm);

		run_cycles -= CYCLE - start_cycle;
	}while(0 < run_cycles);

	return(run_cycles);
}

void armvm_step(armvm_p const avm)
{ return(armvm_core_step(avm->core)); }
