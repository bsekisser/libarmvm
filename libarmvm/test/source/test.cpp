#include "test.hpp"

/* **** */

#include "test_arm.hpp"
#include "test_armvm_mem.h"
#include "test_utility.hpp"

/* **** */

extern "C" {
	#include "libarmvm/include/armvm_glue.h"
	#include "libarmvm/include/armvm_core.h"
	#include "libarmvm/include/armvm_mem.h"
	#include "libarmvm/include/armvm_trace.h"
	#include "libarmvm/include/armvm.h"
}

/* **** */

extern "C" {
	#include "libarm/include/arm_cpsr.h"
}

/* **** */

extern "C" {
	#include "libbse/include/err_test.h"
	#include "libbse/include/log.h"
	#include "libbse/include/shift_roll.h"
	#include "libbse/include/unused.h"
}

/* **** */

extern "C" {
	#include <assert.h>
}

/* **** */

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
		armvm_core_step(pARMVM_CORE);
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

int test::check_nz(int n, int z)
{
	unsigned xpsr = 0;

	ARM_CPSRx_BMAS(xpsr, N, !!n);
	ARM_CPSRx_BMAS(xpsr, Z, !!z);

	return(_test_cpsr_xpsr_mask(t, CPSR, xpsr, ARM_CPSR_MASK_NZ));
}

int test::check_nzc(int n, int z, int c)
{
	unsigned xpsr = 0;

	ARM_CPSRx_BMAS(xpsr, N, !!n);
	ARM_CPSRx_BMAS(xpsr, Z, !!z);
	ARM_CPSRx_BMAS(xpsr, C, !!c);

	return(_test_cpsr_xpsr_mask(t, CPSR, xpsr, ARM_CPSR_MASK_NZC));
}

int test::check_nzcv(int n, int z, int c, int v)
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

	class test_arm ta;
	ta.run_tests();

//	test_armvm_mem(t);

	return(0);
	UNUSED(argc, argv);
}

void test::reset(void)
{
if(0) LOG("GEN_COUNT: 0x%08x, GEN_PC: 0x%08x, RUN_PC: 0x%08x", GEN_COUNT, GEN_PC, RUN_PC);

	armvm_reset(pARMVM);

if(0) LOG("GEN_COUNT: 0x%08x, GEN_PC: 0x%08x, RUN_PC: 0x%08x", GEN_COUNT, GEN_PC, RUN_PC);

	assert(0 == RUN_PC);

	GEN_COUNT = 0;
	GEN_PC = RUN_PC;
}

uint32_t test::run_test(void)
{ return(_run_test(t, 0)); }

uint32_t test::run_test_flags(const uint32_t flag_set)
{ return(_run_test(t, flag_set)); }

uint32_t test::step_test(void)
{
	TEST_PC = RUN_PC;

	if(GEN_COUNT) {
		armvm_core_step(pARMVM_CORE);
		GEN_COUNT--;
	}

	GEN_PC = RUN_PC;
	return(RUN_PC);
}

test::test():cc(mem)
{
	t = &tt;
	t->cc = cc.p2armcc_t();

	armvm_alloc_init(armvm_alloc(&pARMVM));
	ERR_NULL(pARMVM);

	pARMVM_CORE->config.trace = 1;

	armvm_mem_mmap_rw(pARMVM_MEM, 0, sizeof(mem), mem);
}

test::~test()
{
	armvm_exit(pARMVM);
}
