#include "armvm_mmu.h"

/* **** */

#include "armvm_action.h"
#include "armvm_coprocessor_cp15.h"
#include "armvm_core.h"
#include "armvm_core_exception.h"
#include "armvm_mem.h"
#include "armvm_tlb.h"
#include "armvm.h"

/* **** */

#include "libbse/include/handle.h"
#include "libbse/include/log.h"
#include "libbse/include/page.h"
#include "libbse/include/unused.h"

/* **** */

typedef struct armvm_mmu_t {
	armvm_mem_callback_p l1[PAGE_SIZE];
//
	armvm_p armvm;
#define pARMVM mmu->armvm

	armvm_core_p core;
#define pARMVM_CORE mmu->core

	armvm_mmu_h h2mmu;
	armvm_mem_p mem;
	armvm_tlb_p tlb;
}armvm_mmu_t;

/* **** */

static void __armvm_mmu_alloc_init(armvm_mmu_p const mmu)
{
	pARMVM_CORE = mmu->armvm->core;
	mmu->mem = mmu->armvm->mem;
}

static void __armvm_mmu_exit(armvm_mmu_p mmu)
{
	if(mmu->armvm->config.trace.exit) LOG();

	handle_free((void*)mmu->h2mmu);
}

/* **** */

void armvm_mmu(armvm_mmu_p const mmu, const unsigned action)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: __armvm_mmu_alloc_init(mmu); break;
	}

	armvm_tlb(mmu->tlb, action);

	switch(action) {
		case ARMVM_ACTION_EXIT: __armvm_mmu_exit(mmu); break;
	}
}

static int armvm_mmu__vpa2ppa(armvm_mmu_p const mmu, uint32_t *const ppa)
{ LOG_ACTION(exit(-1)); UNUSED(mmu, ppa); } // TODO

armvm_mmu_p armvm_mmu_alloc(armvm_p const avm, armvm_mmu_h const h2mmu)
{
	ERR_NULL(h2mmu);
	ERR_NULL(avm);

	if(avm->config.trace.alloc) LOG();

	armvm_mmu_p mmu = handle_calloc((void*)h2mmu, 1, sizeof(armvm_mmu_t));

	mmu->armvm = avm;
	mmu->h2mmu = h2mmu;

	/* **** */

	ERR_NULL(armvm_tlb_alloc(avm, mmu, &mmu->tlb));

	/* **** */

	return(mmu);
}

int armvm_mmu_ifetch(armvm_mmu_p const mmu, uint32_t *const ir,
	const uint32_t va, const size_t size)
{
	uint32_t ppa = va;
	armvm_mem_callback_p src = 0;
	int tlb = 0;
	armvm_tlbe_p tlbe = 0;

	if(CP15_REG1_BIT(M)) {
		src = armvm_tlb_ifetch(mmu->tlb, va, &tlbe);

		if(src) {
			*ir = armvm_mem_callback_io(src, va, size, 0);
			return(1);
		}

		tlb = armvm_mmu__vpa2ppa(mmu, &ppa);
	}

	*ir = armvm_mem_access_read(mmu->mem, ppa, size, &src);
	if(!src)
		return(armvm_core_exception_prefetch_abort(pARMVM_CORE));

	if(tlb && src)
		armvm_tlb_fill_instruction_tlbe(tlbe, va, src);

	return(1);
}

int armvm_mmu_read(armvm_mmu_p const mmu, uint32_t *const read,
	const uint32_t va, const size_t size)
{
	uint32_t ppa = va;
	armvm_mem_callback_p src = 0;
	int tlb = 0;
	armvm_tlbe_p tlbe = 0;

	if(CP15_REG1_BIT(M)) {
		src = armvm_tlb_read(mmu->tlb, va, &tlbe);

		if(src) {
			*read = armvm_mem_callback_io(src, va, size, 0);
			return(1);
		}

		tlb = armvm_mmu__vpa2ppa(mmu, &ppa);
	}

	*read = armvm_mem_access_read(mmu->mem, ppa, size, &src);
	if(!src)
		return(armvm_core_exception_data_abort(pARMVM_CORE));

	if(tlb && src)
		armvm_tlb_fill_data_tlbe(tlbe, va, src);

	return(1);
}

int armvm_mmu_write(armvm_mmu_p const mmu, const uint32_t va,
	const size_t size, const uint32_t write)
{
	uint32_t ppa = va;
	armvm_mem_callback_p dst = 0;
	int tlb = 0;
	armvm_tlbe_p tlbe = 0;

	if(CP15_REG1_BIT(M)) {
		dst = armvm_tlb_write(mmu->tlb, va, &tlbe);

		if(dst) {
			armvm_mem_callback_io(dst, va, size, (uint32_t*)&write);
			return(1);
		}

		tlb = armvm_mmu__vpa2ppa(mmu, &ppa);
	}

	dst = armvm_mem_access_write(mmu->mem, ppa, size, write);
	if(!dst)
		return(armvm_core_exception_data_abort(pARMVM_CORE));

	if(tlb && dst)
		armvm_tlb_fill_data_tlbe(tlbe, va, dst);

	return(1);
}
