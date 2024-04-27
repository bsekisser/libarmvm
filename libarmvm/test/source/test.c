#include "test.h"

/* **** */

#include "test_arm.h"
#include "test_armvm_mem.h"
#include "test_utility.h"

/* **** */

#include "libarmvm/include/armvm_glue.h"
#include "libarmvm/include/armvm_core.h"
#include "libarmvm/include/armvm_mem.h"
#include "libarmvm/include/armvm_trace.h"
#include "libarmvm/include/armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/shift_roll.h"
#include "libbse/include/unused.h"

/* **** */

#include "assert.h"

/* **** */

test_t tt, *t = &tt;

#define LOGx(_x) LOG(#_x " = 0x%08x", _x);

static void _preflight_tests(void)
{
	assert(-1U == ~0U);
	assert(1 == (0 == 0));
	assert(0 == (0 != 0));
	assert(1 == !!1);
	assert(0 == !!0);
	assert(1 == !!-1);
	assert(1 == !!0x12345678);
	assert(1 == !!0x87654321);
	assert(0x01 == sizeof(uint8_t));
	assert(0x02 == sizeof(uint16_t));
	assert(0x04 == sizeof(uint32_t));
	assert(0x08 == sizeof(uint64_t));
	assert(sizeof(uint32_t) <= sizeof(signed));
	assert(sizeof(uint32_t) <= sizeof(unsigned));

//	LOGx(_ror(64, 0));
//	LOGx(_ror(64, 26));
//	LOGx(_ror(64, 28));
//	LOG((0xe << 1));
//	LOGx(_ror(0x3f, (0xe << 1)));
//	LOG((0xf << 1));
//	LOGx(_ror(0xfc, (0xf << 1)));

#if 0
	/*
	 * https://stackoverflow.com/a/60023331
	 *
	 * >> x promoted to signed int
	 */

	uint16_t x = 0xf123;
	uint32_t y = (x << 16) >> 16;

	LOG("x = 0x%08x, y = 0x%08x", x, y);

	assert(x == y);
#endif

	for(int i = 1; i < 32; i++) {
		uint32_t check1 = (32 - i);
		uint32_t check2 = (-i & 31);
		if(0) LOG("((32 - i) == (-i & 31)) -- (0x%08x, 0x%08x)",
			check1, check2);
		assert(check1 == check2);
	}
}

static uint32_t _run_test(test_p t, const uint32_t flag_set)
{
	CPSR &= ~ARM_CPSR_MASK_NZCV;
	CPSR |= flag_set;

	TEST_PC = RUN_PC;

	while(GEN_COUNT) {
		armvm_core_step(pCORE);
		GEN_COUNT--;
	}

	GEN_PC = RUN_PC;
	return(RUN_PC);
}

static int _test_cpsr_xpsr_mask(test_p t, unsigned cpsr, unsigned xpsr, unsigned mask)
{
	unsigned test_cpsr = cpsr & mask;
	unsigned test_xpsr = xpsr & mask;

	if(test_cpsr != test_xpsr) {
		TRACE_PSR(cpsr);
		TRACE_PSR(xpsr);

		return(0);
	}

	return(1);

	UNUSED(t);
}

static int _test_cpsr_xpsr(test_p t, unsigned cpsr, unsigned xpsr)
{
	return(_test_cpsr_xpsr_mask(t, cpsr, xpsr, ARM_CPSR_MASK_NZCV));
}

int check_nz(test_p t, int n, int z)
{
	unsigned xpsr = 0;

	ARM_CPSRx_BMAS(xpsr, N, !!n);
	ARM_CPSRx_BMAS(xpsr, Z, !!z);

	return(_test_cpsr_xpsr_mask(t, CPSR, xpsr, ARM_CPSR_MASK_NZ));
}

int check_nzc(test_p t, int n, int z, int c)
{
	unsigned xpsr = 0;

	ARM_CPSRx_BMAS(xpsr, N, !!n);
	ARM_CPSRx_BMAS(xpsr, Z, !!z);
	ARM_CPSRx_BMAS(xpsr, C, !!c);

	return(_test_cpsr_xpsr_mask(t, CPSR, xpsr, ARM_CPSR_MASK_NZC));
}

int check_nzcv(test_p t, int n, int z, int c, int v)
{
	unsigned xpsr = 0;

	ARM_CPSRx_BMAS(xpsr, N, !!n);
	ARM_CPSRx_BMAS(xpsr, Z, !!z);
	ARM_CPSRx_BMAS(xpsr, C, !!c);
	ARM_CPSRx_BMAS(xpsr, V, !!v);

	return(_test_cpsr_xpsr(t, CPSR, xpsr));
}

int main(int argc, char** argv)
{
	_preflight_tests();

//	armvm_trace_t armvm_trace;

	armvm_alloc_init(armvm_alloc(&pARMVM));

	pARMVM->core->config.trace = 1;
//	pARMVM->core->armvm_trace = &armvm_trace;
//	armvm_trace.armvm_core = pARMVM->core;

	armvm_mem_mmap(pARMVM->mem, 0, sizeof(t->mem), 0, t->mem);

	test_armvm_mem(t);

	test_arm(t);

	armvm_exit(pARMVM);
}

void reset(test_p t)
{
if(0) LOG("GEN_COUNT: 0x%08x, GEN_PC: 0x%08x, RUN_PC: 0x%08x", GEN_COUNT, GEN_PC, RUN_PC);

	armvm_reset(pARMVM);

if(0) LOG("GEN_COUNT: 0x%08x, GEN_PC: 0x%08x, RUN_PC: 0x%08x", GEN_COUNT, GEN_PC, RUN_PC);

	assert(0 == RUN_PC);

	GEN_COUNT = 0;
	GEN_PC = RUN_PC;
}

uint32_t run_test(test_p t)
{ return(_run_test(t, 0)); }

uint32_t run_test_flags(test_p t, const uint32_t flag_set)
{ return(_run_test(t, flag_set)); }

uint32_t step_test(test_p t)
{
	TEST_PC = RUN_PC;

	if(GEN_COUNT) {
		armvm_core_step(pCORE);
		GEN_COUNT--;
	}

	GEN_PC = RUN_PC;
	return(RUN_PC);
}
