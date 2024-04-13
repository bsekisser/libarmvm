#include "armvm_core.h"

/* **** */

#include "armvm_core_arm.h"
#include "armvm_core_config.h"
#include "armvm_core_exception.h"
#include "armvm_core_glue.h"
#include "armvm_core_thumb.h"

/* **** */

#include "armvm_action.h"
#include "armvm.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"

/* **** */

static void __armvm_core_alloc_init(armvm_core_p const core)
{
	core->cp = core->armvm->coprocessor;
	core->mmu = core->armvm->mmu;

	/* **** */

	if(core->armvm->config.trace.alloc) LOG();

	core->config.pedantic.ir_checks = 1;
	core->config.version = arm_v5tej;

	switch(core->config.version) {
		case arm_v5tej:
			core->config.features.thumb = 1;
		break;
	}
}

static void __armvm_core_exit(armvm_core_p const core)
{
	if(core->armvm->config.trace.exit) LOG();

	handle_free((void*)core->h2core);
}

/* **** */

void armvm_core(armvm_core_p const core, const unsigned action)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: __armvm_core_alloc_init(core); break;
		case ARMVM_ACTION_RESET: armvm_core_exception_reset(core); break;
	}
//
//
	switch(action) {
		case ARMVM_ACTION_EXIT: __armvm_core_exit(core); break;
	}
}

armvm_core_p armvm_core_alloc(armvm_p const avm, armvm_core_h const h2core)
{
	ERR_NULL(avm);
	ERR_NULL(h2core);

	if(avm->config.trace.alloc) LOG();

	armvm_core_p core = handle_calloc((void*)h2core, 1, sizeof(armvm_core_t));
	ERR_NULL(core);

	/* **** */

	core->armvm = avm;
	core->h2core = h2core;

	/* **** */

	return(core);
}

void armvm_core_step(armvm_core_p const core)
{
	CYCLE++;
	ICOUNT++;

	if(1 & PC)
		return(armvm_core_thumb_step(core));

	return(armvm_core_arm_step(core));
}
