extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	#include <stdint.h>
}

/* **** */

extern "C" {
	int test_arm_b_bl(test_p t)
{
	reset(t);

	b(0x10);
	run_test(t);
	fail_if(0x10 != rR(PC));

	bl(0x20);
	run_test(t);
	fail_if(0x20 != rR(PC));
	fail_if(4 != (rR(LR) - TEST_PC));

	const uint32_t bl_LR = rR(LR);

	blx(rLR);
	run_test(t);
	fail_if(bl_LR != rR(PC));
	fail_if(4 != (rR(LR) - TEST_PC));
	fail_if(ARM_CPSR_BEXT(Thumb));

	const uint32_t blx_LR = rR(LR);

	bx(rLR);
	run_test(t);
	fail_if(blx_LR != rR(PC));
	fail_if(ARM_CPSR_BEXT(Thumb));

	blx(0x43);
	run_test(t);
	fail_if(0x42 != rR(PC));
	fail_if(!ARM_CPSR_BEXT(Thumb));

	return(1);
}}
