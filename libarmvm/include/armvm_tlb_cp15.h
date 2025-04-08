#pragma once

/* **** */

static void armvm_tlb_invalidate_all(armvm_tlb_ref tlb);
static void armvm_tlb_invalidate_data(armvm_tlb_ref tlb);
static void armvm_tlb_invalidate_instruction(armvm_tlb_ref tlb);

/* **** */

static uint32_t _armvm_cp15_0_8_5_0_invalidate_instruction(void *const param, uint32_t *const write)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate instruction TLB");
		armvm_tlb_invalidate_instruction(param);
	} else {
		DEBUG(LOG("XX READ -- Invalidate instruction TLB"));
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
		DEBUG(LOG("XX READ -- Invalidate data TLB"));
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
		DEBUG(LOG("XX READ -- Invalidate TLB"));
	}

	return(data);
}

static void _armvm_tlb_cp15_init(armvm_tlb_ref tlb)
{
	armvm_coprocessor_p cp = tlb->armvm->coprocessor;

	armvm_coprocessor_register_callback(cp, cp15(0, 8, 5, 0),
		_armvm_cp15_0_8_5_0_invalidate_instruction, tlb);
	armvm_coprocessor_register_callback(cp, cp15(0, 8, 6, 0),
		_armvm_cp15_0_8_6_0_invalidate_data, tlb);
	armvm_coprocessor_register_callback(cp, cp15(0, 8, 7, 0),
		_armvm_cp15_0_8_7_0_invalidate_all, tlb);
}
