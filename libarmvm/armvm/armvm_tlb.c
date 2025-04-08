#include "armvm_tlb.h"

/* **** */

#include "armvm_mem.h"
#include "armvm_mmu.h"
#include "armvm_coprocessor.h"
#include "armvm_coprocessor_glue.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/bitfield.h"
#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"
#include "libbse/include/page.h"

/* **** */

#define dTLB_BITS 8
#define iTLB_BITS 8

typedef struct armvm_tlbe_tag {
	armvm_mem_callback_p cb;
//
	struct {
		uint32_t i:1;
		uint32_t vp:20;
	};
}armvm_tlbe_t;

typedef struct armvm_tlb_tag {
	armvm_tlbe_t dtlb[_BV(dTLB_BITS)];
	armvm_tlbe_t itlb[_BV(iTLB_BITS)];
//
	armvm_p armvm;
	armvm_tlb_hptr h2tlb;
	armvm_mmu_p mmu;
}armvm_tlb_t;

/* **** forward declarations */

#include "armvm_tlb_cp15.h"

/* **** */

static void _armvm_tlb_exit(armvm_tlb_ref tlb)
{
	if(action_log.at.exit) LOG();
	ERR_NULL(tlb);

	handle_free((void*)tlb->h2tlb);
}

static void _armvm_tlb_init(armvm_tlb_ref tlb)
{
	if(action_log.at.init) LOG();
	ERR_NULL(tlb);

	/* **** */

	_armvm_tlb_cp15_init(tlb);
}

/* **** */

static armvm_tlbe_ptr _tlb_entry(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits,
	const uint32_t va, armvm_tlbe_href h2tlbe)
{
	const uint32_t vp = PAGE(va);
	const uint32_t vp_tlbe = vp & _BM(tlb_bits);

	armvm_tlbe_ref tlbe = &tlbe_table[vp_tlbe];

	if(h2tlbe)
		*h2tlbe = tlbe;

	if(!tlbe->i || (vp != tlbe->vp))
		return(0);

	return(tlbe);
}

static void _tlb_fill_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_p const cb)
{
	tlbe->cb = cb;
	tlbe->vp = PAGE(va);

	tlbe->i = 1;
}

static void _tlb_invalidate_table(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits)
{
	for(unsigned i = 0; i < _BV(tlb_bits); i++)
		memset(&tlbe_table[i], 0, sizeof(armvm_tlbe_t));
}

static armvm_mem_callback_p _tlb_read(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits,
	const uint32_t va, armvm_tlbe_href h2tlbe)
{
	armvm_tlbe_ref tlbe = _tlb_entry(tlbe_table, tlb_bits, va, h2tlbe);

	if(!tlbe) return(0);

	return(tlbe->cb);
}

static armvm_mem_callback_p _tlb_write(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits,
	const uint32_t va, armvm_tlbe_href h2tlbe)
{
	armvm_tlbe_ref tlbe = _tlb_entry(tlbe_table, tlb_bits, va, h2tlbe);

	if(!tlbe) return(0);

	return(tlbe->cb);
}

/* **** */

void armvm_tlb(armvm_tlb_ref tlb, action_ref action)
{
	switch(action) {
//		case _ACTION_ALLOC_INIT: return(_armvm_tlb_alloc_init(tlb));
		case _ACTION_INIT: return(_armvm_tlb_init(tlb));
		default: break;
	}

	switch(action) {
		case _ACTION_EXIT: return(_armvm_tlb_exit(tlb));
		default: break;
	}
}

armvm_tlb_ptr armvm_tlb_alloc(armvm_p const avm, armvm_mmu_p const mmu, armvm_tlb_href h2tlb)
{
	if(action_log.at.alloc) LOG();

	ERR_NULL(h2tlb);
	ERR_NULL(avm);

	/* **** */

	armvm_tlb_ref tlb = handle_calloc((void*)h2tlb, 1, sizeof(armvm_tlb_t));
	ERR_NULL(tlb);

	/* **** */

	tlb->armvm = avm;
	tlb->h2tlb = h2tlb;
	tlb->mmu = mmu;

	/* **** */

	return(tlb);
}

void armvm_tlb_fill_data_tlbe(armvm_tlbe_ref tlbe, const uint32_t va,
	armvm_mem_callback_p const cb)
{ _tlb_fill_tlbe(tlbe, va, cb); }

void armvm_tlb_fill_instruction_tlbe(armvm_tlbe_ref tlbe, const uint32_t va,
	armvm_mem_callback_p const cb)
{ _tlb_fill_tlbe(tlbe, va, cb); }

armvm_mem_callback_p armvm_tlb_ifetch(armvm_tlb_ref tlb, const uint32_t va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_p cb = _tlb_read(tlb->itlb, iTLB_BITS, va, h2tlbe);

	return(cb);
}

static void armvm_tlb_invalidate_all(armvm_tlb_ref tlb)
{
	armvm_tlb_invalidate_data(tlb);
	armvm_tlb_invalidate_instruction(tlb);
}

static void armvm_tlb_invalidate_data(armvm_tlb_ref tlb)
{
	_tlb_invalidate_table(tlb->dtlb, dTLB_BITS);
}

static void armvm_tlb_invalidate_instruction(armvm_tlb_ref tlb)
{
	_tlb_invalidate_table(tlb->itlb, iTLB_BITS);
}

armvm_mem_callback_p armvm_tlb_read(armvm_tlb_ref tlb, uint32_t const va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_p cb = _tlb_read(tlb->dtlb, dTLB_BITS, va, h2tlbe);

	return(cb);
}

armvm_mem_callback_p armvm_tlb_write(armvm_tlb_ref tlb, uint32_t const va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_p cb = _tlb_write(tlb->dtlb, dTLB_BITS, va, h2tlbe);

	return(cb);
}
