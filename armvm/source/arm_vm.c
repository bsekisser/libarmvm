#include "armvm_config.h"
#include "armvm.h"

/* **** */

#include "armvm_core_arm.h"
#include "armvm_core_glue.h"
#include "armvm_core_thumb.h"

/* **** */

#include "arm_cpsr.h"

/* **** */

#include "err_test.h"
#include "handle.h"
#include "log.h"

/* **** */

armvm_p armvm_alloc(armvm_h h2avm)
{
	ERR_NULL(h2avm);

	return(armvm_alloc_init(handle_calloc((void*)h2avm, 1, sizeof(armvm_t))));
}

armvm_p armvm_alloc_init(armvm_p avm)
{
	ERR_NULL(avm);

	/* **** */

	vmCONFIG->pedantic.ir_checks = 1;
	vmCONFIG->version = arm_v5tej;

	switch(vmCONFIG->version) {
		case arm_v5tej:
			vmCONFIG->features.thumb = 1;
		break;
	}

	/* **** */

	return(avm);
}

armvm_p armvm_init(armvm_p avm)
{
//	avm->step = armvm_step;

	PC = 0;

	return(avm);
}

void armvm_reset(armvm_p avm)
{
	for(unsigned r = 0; r < 16; r++)
		GPRx(r) = 0;

	CPSR = ARM_CPSR_M32(Supervisor);
	PC = 0;
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
		return(armvm_step_thumb(avm));

	return(armvm_step_arm(avm));
}
