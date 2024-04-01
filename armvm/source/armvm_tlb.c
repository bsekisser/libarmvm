#include "armvm_tlb.h"

/* **** */

#include "armvm_mem.h"
#include "armvm_coprocessor.h"
#include "armvm_coprocessor_glue.h"
#include "armvm_action.h"
#include "armvm.h"

/* **** */

#include "git/libbse/include/bitfield.h"
#include "git/libbse/include/err_test.h"
#include "git/libbse/include/handle.h"

/* **** */

#define dTLB_BITS 8
#define iTLB_BITS 8

typedef struct armvm_tlbe_t* armvm_tlbe_p;
typedef struct armvm_tlbe_t {
	armvm_mem_callback_p cb;
//
	struct {
		uint32_t pa:20;
	};
}armvm_tlbe_t;

typedef struct armvm_tlb_t {
	armvm_tlbe_t dtlb[_BV(dTLB_BITS)];
	armvm_tlbe_t itlb[_BV(iTLB_BITS)];
//
	armvm_p armvm;
	armvm_tlb_h h2tlb;
}armvm_tlb_t;

/* **** forward declarations */

/* **** */

static void ___invalidate_table(unsigned tlb_bits, armvm_tlbe_p tlbe_table)
{
	for(unsigned i = 0; i < _BV(tlb_bits); i++)
		memset(&tlbe_table[i], 0, sizeof(armvm_tlbe_t));
}

/* **** */

static void __invalidate_data(armvm_tlb_p tlb)
{
	___invalidate_table(dTLB_BITS, tlb->dtlb);
}

static void __invalidate_instruction(armvm_tlb_p tlb)
{
	___invalidate_table(iTLB_BITS, tlb->itlb);
}

static void __invalidate_all(armvm_tlb_p tlb)
{
	__invalidate_data(tlb);
	__invalidate_instruction(tlb);
}

/* **** */

static void _armvm_tlb_exit(armvm_tlb_p tlb)
{
	ERR_NULL(tlb);

	if(tlb->armvm->config.trace.exit) LOG();

	handle_free((void*)tlb->h2tlb);
}

/* **** */

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

static void _armvm_tlb_init(armvm_tlb_p tlb)
{
	ERR_NULL(tlb);

	if(tlb->armvm->config.trace.init) LOG();

	/* **** */

	armvm_coprocessor_p cp = tlb->armvm->coprocessor;

	armvm_coprocessor_register_callback(cp15(0, 8, 5, 0),
		_cp15_0_8_5_0_invalidate_instruction, tlb, cp);
	armvm_coprocessor_register_callback(cp15(0, 8, 6, 0),
		_cp15_0_8_6_0_invalidate_data, tlb, cp);
	armvm_coprocessor_register_callback(cp15(0, 8, 7, 0),
		_cp15_0_8_7_0_invalidate_all, tlb, cp);
}

/* **** */

armvm_tlb_p armvm_tlb_alloc(armvm_tlb_h h2tlb, armvm_p avm)
{
	ERR_NULL(h2tlb);
	ERR_NULL(avm);

	if(avm->config.trace.alloc) LOG();

	/* **** */

	armvm_tlb_p tlb = handle_calloc((void*)h2tlb, 1, sizeof(armvm_tlb_t));
	ERR_NULL(tlb);

	tlb->armvm = avm;
	tlb->h2tlb = h2tlb;

	/* **** */
	return(tlb);
}

void armvm_tlb(unsigned action, armvm_tlb_p tlb)
{
	switch(action) {
		case ARMVM_ACTION_EXIT: return(_armvm_tlb_exit(tlb));
		case ARMVM_ACTION_INIT: return(_armvm_tlb_init(tlb));
	}
}
