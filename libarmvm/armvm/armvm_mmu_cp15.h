#pragma once

/* **** */

#undef DEBUG
#define DEBUG(_x) _x

#ifndef DEBUG
	#define DEBUG(...)
#endif

static uint32_t _mmu_cp15__0_2_0_0__ttbr(armvm_mmu_ref mmu, uint32_t *const write, const unsigned r)
{
	const uint32_t mask = mlBF(31, 14 - TTBCR_N) | mlBF(4, 3) | _BV(2) | _BV(1) | _BV(0);

	uint32_t* p2ttbr = armvm_coprocessor_cp15r_rmw(mmu->cp, rSPR32(IR));

	const uint32_t data = write ? *write : *p2ttbr;

	if(write) {
		LOG_START("Translation Table Base %01u\n\t", r);
		_LOG_("mmu->ttbr[0]: 0x%08x", mlBFTST(data, 31, 14 - TTBCR_N));
		_LOG_(" SBZ: 0x%03x", mlBFEXT(data, 13, 5));
		_LOG_(" RGN: %01u", mlBFEXT(data, 4, 3));
		_LOG_(" IMP: %01u", BEXT(data, 2));
		_LOG_(" %c", BEXT(data, 1) ? 'S' : 's');
		LOG_END(" %c", BEXT(data, 0) ? 'C' : 'c');

		*p2ttbr = data & mask;
	} else {
		DEBUG(LOG("READ -- Translation Table Base %01u", r));
	}

	return(data);
}

static uint32_t _mmu_cp15__0_2_0_0_ttbr0(void *const param, uint32_t *const write)
{ return(_mmu_cp15__0_2_0_0__ttbr(param, write, 0)); }

static uint32_t _mmu_cp15__0_2_0_1_ttbr1(void *const param, uint32_t *const write)
{ return(_mmu_cp15__0_2_0_0__ttbr(param, write, 0)); }

static uint32_t _mmu_cp15__0_2_0_2_ttbcr(void *const param, uint32_t *const write)
{
	armvm_mmu_ref mmu = param;

	const uint32_t ttbcr = armvm_coprocessor_cp15r(mmu->cp, rSPR32(IR), write);

	if(write) {
		assert(0 == mlBFEXT(rSPR32(IR), 31, 3));

		LOG("n: %u", mlBFEXT(ttbcr, 2, 0));
	}

	return(ttbcr);
}

static void _mmu_cp15_init(armvm_mmu_ref mmu)
{
	armvm_coprocessor_register_callback(mmu->cp, cp15(0, 2, 0, 0), _mmu_cp15__0_2_0_0_ttbr0, mmu);
	armvm_coprocessor_register_callback(mmu->cp, cp15(0, 2, 0, 1), _mmu_cp15__0_2_0_1_ttbr1, mmu);
	armvm_coprocessor_register_callback(mmu->cp, cp15(0, 2, 0, 2), _mmu_cp15__0_2_0_2_ttbcr, mmu);
}
