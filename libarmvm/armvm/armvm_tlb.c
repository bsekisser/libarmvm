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

static void _tlb_fill_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_ref cb)
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

static armvm_mem_callback_ptr _tlb_read(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits,
	const uint32_t va, armvm_tlbe_href h2tlbe)
{
	armvm_tlbe_ref tlbe = _tlb_entry(tlbe_table, tlb_bits, va, h2tlbe);

	if(!tlbe) return(0);

	return(tlbe->cb);
}

static armvm_mem_callback_ptr _tlb_write(armvm_tlbe_ref tlbe_table, const unsigned tlb_bits,
	const uint32_t va, armvm_tlbe_href h2tlbe)
{
	armvm_tlbe_ref tlbe = _tlb_entry(tlbe_table, tlb_bits, va, h2tlbe);

	if(!tlbe) return(0);

	return(tlbe->cb);
}

/* **** */

static
int armvm_tlb_action_exit(int err, void *const param, action_ref)
{
	ACTION_LOG(exit);

	/* **** */

	handle_ptrfree(param);

	/* **** */

	return(err);
}

armvm_tlb_ptr armvm_tlb_alloc(armvm_ref avm, armvm_mmu_ref mmu, armvm_tlb_href h2tlb)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2tlb);
	ERR_NULL(avm);

	/* **** */

	armvm_tlb_ref tlb = handle_calloc(h2tlb, 1, sizeof(armvm_tlb_t));
	ERR_NULL(tlb);

	/* **** */

	tlb->armvm = avm;
	tlb->mmu = mmu;

	/* **** */

	return(tlb);
}

void armvm_tlb_fill_data_tlbe(armvm_tlbe_ref tlbe, const uint32_t va,
	armvm_mem_callback_ref cb)
{ _tlb_fill_tlbe(tlbe, va, cb); }

void armvm_tlb_fill_instruction_tlbe(armvm_tlbe_ref tlbe, const uint32_t va,
	armvm_mem_callback_ref cb)
{ _tlb_fill_tlbe(tlbe, va, cb); }

armvm_mem_callback_ptr armvm_tlb_ifetch(armvm_tlb_ref tlb, const uint32_t va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_ptr cb = _tlb_read(tlb->itlb, iTLB_BITS, va, h2tlbe);

	return(cb);
}

void armvm_tlb_invalidate_all(armvm_tlb_ref tlb)
{
	armvm_tlb_invalidate_data(tlb);
	armvm_tlb_invalidate_instruction(tlb);
}

void armvm_tlb_invalidate_data(armvm_tlb_ref tlb)
{
	_tlb_invalidate_table(tlb->dtlb, dTLB_BITS);
}

void armvm_tlb_invalidate_instruction(armvm_tlb_ref tlb)
{
	_tlb_invalidate_table(tlb->itlb, iTLB_BITS);
}

armvm_mem_callback_ptr armvm_tlb_read(armvm_tlb_ref tlb, uint32_t const va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_ptr cb = _tlb_read(tlb->dtlb, dTLB_BITS, va, h2tlbe);

	return(cb);
}

armvm_mem_callback_ptr armvm_tlb_write(armvm_tlb_ref tlb, uint32_t const va,
	armvm_tlbe_href h2tlbe)
{
	armvm_mem_callback_ptr cb = _tlb_write(tlb->dtlb, dTLB_BITS, va, h2tlbe);

	return(cb);
}

static
action_handler_t armvm_tlb_action_sublist[] = {
	{{ .list = &armvm_tlb_cp15_action_list }, { .is_list = 1 }, 0 },
	{{ 0 }, { 0 }, 0 },
};

ACTION_LIST(armvm_tlb_action_list,
	.list = {
		[_ACTION_EXIT] = {{ armvm_tlb_action_exit }, { 0 }, 0 },
	},

	SUBLIST(armvm_tlb_action_sublist),
);
