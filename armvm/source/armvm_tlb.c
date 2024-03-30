#include "armvm_tlb.h"

/* **** */

#include "armvm_mem.h"
#include "armvm_state.h"
#include "armvm.h"

/* **** */

#include "bitfield.h"
#include "err_test.h"
#include "handle.h"

/* **** */

#define dTLB_BITS 8
#define iTLB_BITS 8

typedef struct armvm_tlbe_t {
	armvm_mem_callback_p cb;
}armvm_tlbe_t;

typedef struct armvm_tlb_t {
	armvm_p avm;
	armvm_tlb_h h2tlb;
}armvm_tlb_t;

/* **** forward declarations */

static void armvm_tlb_invalidate_all(soc_tlb_p tlb);
static void armvm_tlb_invalidate_data(soc_tlb_p tlb);

/* **** */

static void _invalidate_all(unsigned tlb_bits, armvm_tlbe_p tlbe_table)
{
	for(unsigned i = 0; i < _BV(tlb_bits); i++)
		memset(&tlbe_table[i], 0, sizeof(soc_tlbe_t));
}


static uint32_t _cp15_0_8_5_0_invalidate_instruction(uint32_t* write, void* param)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate instruction TLB");
		__invalidate_instruction(param);
	} else {
		DEBUG(LOG("XX READ -- Invalidate instruction TLB"));
	}

	return(data);
}

static uint32_t _cp15_0_8_6_0_invalidate_data(uint32_t* write, void* param)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate data TLB");
		__invalidate_data(param);
	} else {
		DEBUG(LOG("XX READ -- Invalidate data TLB"));
	}

	return(data);
}

static uint32_t _cp15_0_8_7_0_invalidate_all(uint32_t* write, void* param)
{
	const uint32_t data = write ? *write : 0;

	if(write) {
		LOG("Invalidate TLB");
		__invalidate_all(param);
	} else {
		DEBUG(LOG("XX READ -- Invalidate TLB"));
	}

	return(data);
}

/* **** */

static void armvm__tlb_exit(armvm_tlb_p tlb)
{
	ERR_NULL(tlb);

	if(tlb->avm->config.trace.exit)
		LOG();

	handle_free(tlb->h2tlb);
}

static void armvm__tlb_init(armvm_tlb_p tlb)
{
	ERR_NULL(tlb);

	if(tlb->avm->config.trace.init)
		LOG();

	/* **** */

	armvm_coprocessor_p cp = tlb->avm->cp;

	armvm_coprocessor_register_access(cp, cp15(0, 8, 5, 0),
		_cp15_0_8_5_0_invalidate_instruction, tlb);
	armvm_coprocessor_register_access(cp, cp15(0, 8, 6, 0),
		_cp15_0_8_6_0_invalidate_data, tlb);
	armvm_coprocessor_register_access(cp, cp15(0, 8, 7, 0),
		_cp15_0_8_7_0_invalidate_all, tlb);
}

/* **** */

armvm_tlb_p armvm_tlb_alloc(armvm_tlb_h h2tlb, armvm_p avm)
{
	ERR_NULL(h2tlb);
	ERR_NULL(avm);

	if(avm->config.trace.alloc)
		LOG();

	/* **** */

	armvm_tlb_p tlb = handle_calloc((void*)h2tlb, 1, sizeof(armvm_tlb_t));
	ERR_NULL(tlb);

	tlb->avm = avm;
	tlb->h2tlb = h2tlb;

	/* **** */
	return(tlb);
}

static void armvm_tlb_invalidate_all(soc_tlb_p tlb)
{
	armvm_tlb_invalidate_data(tlb);
	armvm_tlb_invalidate_instruction(tlb);
}

static void armvm_tlb_invalidate_data(soc_tlb_p tlb)
{
	_invalidate_all(tlb->dtlb, dTLB_BITS);
}

static void armvm_tlb_invalidate_instruction(soc_tlb_p tlb)
{
	_invalidate_all(tlb->itlb, iTLB_BITS);
}

void armvm_tlb(unsigned action, armvm_tlb_p tlb)
{
	switch(actiion) {
		case ARMVM_ACTION_EXIT: return(armvm_tlb_exit(tlb));
		case ARMVM_ACTION_INIT: return(armvm_tlb_init(tlb));
	}
}
