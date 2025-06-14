#include "armvm_cache.h"
#include "armvm_coprocessor.h"
#include "armvm_coprocessor_cp15.h"
#include "armvm_core.h"
#include "armvm_core_exception.h"
#include "armvm_core_glue.h"
#include "armvm_exception.h"

/* **** */

#include "git/libarm/include/arm_cpsr.h"

#include "git/libbse/include/action.h"
#include "git/libbse/include/err_test.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#ifndef DEBUG
	#define DEBUG(_x)
#endif

#define IF_USER_MODE(_action) \
	if(IS_USER_MODE) { \
		LOG_ACTION(_action); \
	}

#define IS_USER_MODE (0 == armvm_core_in_a_privaleged_mode(core))

/* **** */

static
uint32_t _armvm_cp15_0_7_0_4_wait_for_interrupt(void *const param, uint32_t *const write)
{
	armvm_cache_ref acr = param;
	armvm_ref armvm = acr->armvm;
	armvm_core_ref core = acr->core;

	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG(">>: Wait For Interrupt");

		if(0 && !ARM_CPSR_BTST(FIQ)) {
			LOG_ACTION(armvm_exception_fiq(armvm));
		} else if(0 && !ARM_CPSR_BTST(IRQ)) {
			LOG_ACTION(armvm_exception_irq(armvm));
		} else if(0) {
			LOG_ACTION(armvm_core_exception_reset(core));
		} else {
			core->flags.halt = 1;
		}
	} else {
		LOG("<<: Wait For Interrupt");
	}

	return(data);
}

static
uint32_t _armvm_cp15_0_7_10_3_access(void *const param, uint32_t *const write)
{
	uint32_t data = write ? *write : 0;

	if(write) {
		DEBUG(LOG("Cache, Test and Clean"));
	} else {
		LOG("Cache, Test and Clean");
		ARM_CPSRx_BSET(data, Z);
	}

	return(data);
	UNUSED(param);
}

static uint32_t _armvm_cp15_0_7_10_4_access(void *const param, uint32_t *const write)
{
	armvm_cache_ref acr = param;
//	armvm_ref armvm = acr->armvm;
	armvm_core_ref core = acr->core;

	if(write) {
		IF_USER_MODE(armvm_core_exception_undefined_instruction(core));
		LOG("Drain write buffer");
	} else {
		DEBUG(LOG("XX READ -- Drain write buffer"));
	}

	return(0);
}

static uint32_t _armvm_cp15_0_7_5_0_access(void *const param, uint32_t *const write)
{
	armvm_cache_ref acr = param;
//	armvm_ref armvm = acr->armvm;
	armvm_core_ref core = acr->core;

	if(write) {
		IF_USER_MODE(armvm_core_exception_undefined_instruction(core));
		LOG("Invalidate ICache");
	} else {
		DEBUG(LOG("XX READ -- Invalidate ICache"));
	}

	return(0);
}

static uint32_t _armvm_cp15_0_7_6_0_access(void *const param, uint32_t *const write)
{
	armvm_cache_ref acr = param;
//	armvm_ref armvm = acr->armvm;
	armvm_core_ref core = acr->core;

	if(write) {
		IF_USER_MODE(armvm_core_exception_undefined_instruction(core));
		LOG("Invalidate DCache");
	} else {
		DEBUG(LOG("XX READ -- Invalidate DCache"));
	}

	return(0);
}


static uint32_t _armvm_cp15_0_7_7_0_access(void *const param, uint32_t *const write)
{
	armvm_cache_ref acr = param;
//	armvm_ref armvm = acr->armvm;
	armvm_core_ref core = acr->core;

	if(write) {
		IF_USER_MODE(armvm_core_exception_undefined_instruction(core));
		LOG("Invalidate ICache and DCache");
	} else {
		DEBUG(LOG("XX READ -- Invalidate ICache and DCache"));
	}

	return(0);
}

/* **** */

static
int armvm_cache_cp15_action_init(int err, void *const param, action_ref)
{
	ACTION_LOG(init);
	ERR_NULL(param);

	armvm_cache_ref acr = param;

	/* **** */

	ERR_NULL(acr->armvm);

	armvm_coprocessor_ref cp = acr->armvm->coprocessor;
	ERR_NULL(cp);

	armvm_coprocessor_register_callback(cp, cp15(0, 7, 0, 4),
		_armvm_cp15_0_7_0_4_wait_for_interrupt, acr);
	armvm_coprocessor_register_callback(cp, cp15(0, 7, 5, 0),
		_armvm_cp15_0_7_5_0_access, acr);
	armvm_coprocessor_register_callback(cp, cp15(0, 7, 6, 0),
		_armvm_cp15_0_7_6_0_access, acr);
	armvm_coprocessor_register_callback(cp, cp15(0, 7, 7, 0),
		_armvm_cp15_0_7_7_0_access, acr);
	armvm_coprocessor_register_callback(cp, cp15(0, 7, 10, 3),
		_armvm_cp15_0_7_10_3_access, acr);
	armvm_coprocessor_register_callback(cp, cp15(0, 7, 10, 4),
		_armvm_cp15_0_7_10_4_access, acr);

	/* **** */

	return(err);
}

action_list_t armvm_cache_cp15_action_list = {
	.list = {
		[_ACTION_INIT] = {{ armvm_cache_cp15_action_init }, { 0 }, 0 }
	}
};
