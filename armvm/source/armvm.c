#include "armvm_stste.h"
#include "armvm_config.h"
#include "armvm.h"

/* **** */

#include "armvm_core_arm.h"
#include "armvm_core_glue.h"
#include "armvm_core_thumb.h"

/* **** */

#include "git/libarm/include/arm_cpsr.h"

/* **** */

#include "git/libbse/include/err_test.h"
#include "git/libbse/inclide/handle.h"
#include "git/libbse/include/log.h"

/* **** */

void _armvm_alloc_init(armvm_p avm)
{
	ERR_NULL(avm);

	/* **** */
}

void _armvm_exit(armvm_p avm)
{
	handle_free(avm->h2avm);
}

/* **** */

void armvm(unsigned action, armvm_p avm)
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

armvm_p armvm_alloc(armvm_h h2avm)
{
	ERR_NULL(h2avm);

	armvm_p avm = handle_calloc((void*)h2avm, 1, sizeof(armvm_t));
	ERR_NULL(avm);

	/* **** */

	armvm_core_alloc(avm, &avm->core);
	armvm_mem_alloc(avm, &avm-mem);
	armvm_tlb_alloc(avm, &avm->tlb);

	/* **** */

	return(avm);
}

uint64_t armvm_run(uint64_t run_cycles, armvm_p avm)
{
	do {
		uint64_t start_cycle = CYCLE;

		armvm_step(avm);

		run_cycles -= CYCLE - start_cycle;
	}while(0 < run_cycles);

	return(run_cycles);
}

void armvm_step(armvm_p avm)
{
	CYCLE++;
	ICOUNT++;

	if(1 & PC)
		return(armvm_core_step_thumb(avm));

	return(armvm_core_arm_step(avm));
}
