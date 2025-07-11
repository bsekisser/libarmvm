#include "armvm_tlb.h"

#include "armvm_coprocessor.h"
#include "armvm_coprocessor_glue.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/err_test.h"

/* **** */

static uint32_t _armvm_cp15_0_8_5_0_invalidate_instruction(void *const param, uint32_t *const write)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate instruction TLB");
		armvm_tlb_invalidate_instruction(param);
	} else {
;//		DEBUG(LOG("XX READ -- Invalidate instruction TLB"));
	}

	return(data);
}

static uint32_t _armvm_cp15_0_8_6_0_invalidate_data(void *const param, uint32_t *const write)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate data TLB");
		armvm_tlb_invalidate_data(param);
	} else {
;//		DEBUG(LOG("XX READ -- Invalidate data TLB"));
	}

	return(data);
}

static uint32_t _armvm_cp15_0_8_7_0_invalidate_all(void *const param, uint32_t *const write)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate TLB");
		armvm_tlb_invalidate_all(param);
	} else {
;//		DEBUG(LOG("XX READ -- Invalidate TLB"));
	}

	return(data);
}

static
int armvm_tlb_cp15_action_init(int err, void *const param, action_ref)
{
	ACTION_LOG(init);
	ERR_NULL(param);

	armvm_tlb_ref tlb = param;

	/* **** */

	armvm_coprocessor_ref cp = tlb->armvm->coprocessor;

	armvm_coprocessor_register_callback(cp, cp15(0, 8, 5, 0),
		_armvm_cp15_0_8_5_0_invalidate_instruction, tlb);
	armvm_coprocessor_register_callback(cp, cp15(0, 8, 6, 0),
		_armvm_cp15_0_8_6_0_invalidate_data, tlb);
	armvm_coprocessor_register_callback(cp, cp15(0, 8, 7, 0),
		_armvm_cp15_0_8_7_0_invalidate_all, tlb);

	/* **** */

	return(err);
}

ACTION_LIST(armvm_tlb_cp15_action_list,
	.list = {
		[_ACTION_INIT] = {{ armvm_tlb_cp15_action_init }, { 0 }, 0}
	}
);
