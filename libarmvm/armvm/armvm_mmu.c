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

	armvm_coprocessor_p cp;
	armvm_mmu_h h2mmu;
	armvm_mem_p mem;
	armvm_tlb_p tlb;
}armvm_mmu_t;

/* **** */

#define TTBCR(_write) rCP15x(cp15(0, 2, 0, 2), _write)
#define TTBCR_N mlBFEXT(TTBCR(0), 2, 0)

#define TTBR0(_write) rCP15x(cp15(0, 2, 0, 0), _write)
#define TTBR1(_write) rCP15x(cp15(0, 2, 0, 1), _write)

#include "armvm_mmu_cp15.h"

/* **** */

static void __armvm_mmu_alloc_init(armvm_mmu_p const mmu)
{
	armvm_p const avm = mmu->armvm;

	pARMVM_CORE = avm->core;
	mmu->cp = avm->coprocessor;
	mmu->mem = avm->mem;
}

static void __armvm_mmu_init(armvm_mmu_p mmu)
{
	if(mmu->armvm->config.trace.init) LOG();

	_mmu_cp15_init(mmu);
}


static void __armvm_mmu_exit(armvm_mmu_p mmu)
{
	if(mmu->armvm->config.trace.exit) LOG();

	handle_free((void*)mmu->h2mmu);
}

static void __armvm_mmu_reset(armvm_mmu_p const mmu)
{
	if(mmu->armvm->config.trace.reset) LOG();

	uint32_t x = 0;
	TTBCR(&x);

	x = ~0U;
	TTBR0(&x);
	TTBR1(&x);
	CP15_REG1_BCLR(M);
}

#define L1PTD_10_SectionBaseAddress(_x) mlBFTST(_x, 31, 20)
#define L1PTD_10_SectionIndex(_x) mlBFEXT(_x, 19, 0)

static int __l1ptd_10(armvm_mmu_p const mmu, uint32_t *const ppa, const uint32_t l1ptd)
{
	const uint32_t sba = L1PTD_10_SectionBaseAddress(l1ptd);
	const uint32_t sbi = L1PTD_10_SectionIndex(*ppa);

	assert(ppa);
	*ppa = sba | sbi;

	if(0) {
		LOG_START("Section Base Address: 0x%08x", sba);
		_LOG_(", Section Index: 0x%08x", sbi);
		LOG_END(", ppa: 0x%08x", *ppa);
	}

	return(1);
	UNUSED(mmu);
}

#define L1PTD_11_FinePageTable(_x) mlBFTST(_x, 31, 12)
#define L1PTD_11_FinePageTableIndex(_x) mlBFEXT(_x, 19, 10)

static int __l1ptd_11(armvm_mmu_p const mmu, uint32_t *const ppa, const uint32_t l1ptd)
{
	const uint32_t fba = L1PTD_11_FinePageTable(l1ptd);
	const uint32_t fbi = L1PTD_11_FinePageTableIndex(*ppa);

	const uint32_t l2ptd_ppa = fba | fbi;

	if(0) {
		LOG_START("Fine Page Table Base Address: 0x%08x", fba);
		_LOG_(", Index: 0x%08x", fbi);
		LOG_END(", l2ptd_ppa: 0x%08x", l2ptd_ppa);
	}

	const uint32_t l2ptd = armvm_mem_access_read(mmu->mem, l2ptd_ppa, 4, 0);

	switch(l2ptd & 3) {
		default:
			LOG("l2ptd[1:0]: %u", l2ptd & 3);
			LOG_ACTION(exit(-1));
			break;
	}

	return(0);
}

static int __l1ptd_xx(armvm_mmu_p const mmu, uint32_t *const ppa)
{
	const unsigned x = TTBCR_N;

	assert(0 == x);

	const uint32_t ttbr0 = TTBR0(0);

	if(~0U == ttbr0)
		return(0);

	const uint32_t ttb_ppa = mlBFTST(ttbr0, 31, 14 - x);

	const uint32_t ttb_ti = mlBFMOV(*ppa, 31 - x, 20, 2);

	const uint32_t l1ptd_ppa = ttb_ppa | ttb_ti;

	if(0) {
		LOG_START("ttb_ppa: 0x%08x", ttb_ppa);
		_LOG_(", ttb_ti: 0x%08x", ttb_ti);
		LOG_END(", l1ptd_ppa: 0x%08x", l1ptd_ppa);
	}

	const uint32_t l1ptd = armvm_mem_access_read(mmu->mem, l1ptd_ppa, 4, 0);
//	LOG("l1ptd: 0x%08x",  l1ptd);

	switch(l1ptd & 3) {
		case 0b10: return(__l1ptd_10(mmu, ppa, l1ptd));
		case 0b11: return(__l1ptd_11(mmu, ppa, l1ptd));
		default:
			LOG("l1ptd[1:0]: %u", l1ptd & 3);
			LOG_ACTION(exit(-1));
			break;
	}

	return(0);
}

/* **** */

void armvm_mmu(armvm_mmu_p const mmu, const unsigned action)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: __armvm_mmu_alloc_init(mmu); break;
		case ARMVM_ACTION_INIT: __armvm_mmu_init(mmu); break;
		case ARMVM_ACTION_RESET: __armvm_mmu_reset(mmu); break;
	}

	armvm_tlb(mmu->tlb, action);

	switch(action) {
		case ARMVM_ACTION_EXIT: __armvm_mmu_exit(mmu); break;
	}
}

static int armvm_mmu__vpa2ppa(armvm_mmu_p const mmu, uint32_t *const ppa)
{
	if(!CP15_REG1_BIT(M))
		return(0);

	return(__l1ptd_xx(mmu, ppa));
}

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
