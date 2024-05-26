#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

extern "C" {
	#include <stdint.h>
}

/* **** */

int test_arm::b_bl(void)
{
	reset();

	cc.b(0x10);
	run_test();
	fail_if(0x10 != rR(PC));

	cc.bl(0x20);
	run_test();
	fail_if(0x20 != rR(PC));
	fail_if(4 != (rR(LR) - TEST_PC));

	const uint32_t bl_LR = rR(LR);

	cc.blx(rLR);
	run_test();
	fail_if(bl_LR != rR(PC));
	fail_if(4 != (rR(LR) - TEST_PC));
	fail_if(ARM_CPSR_BEXT(Thumb));

	const uint32_t blx_LR = rR(LR);

	cc.bx(rLR);
	run_test();
	fail_if(blx_LR != rR(PC));
	fail_if(ARM_CPSR_BEXT(Thumb));

	cc.bx(rPC);
	run_test();
	fail_if(8 != (rR(PC) - TEST_PC));

	cc.blx(rPC);
	run_test();
	fail_if(4 != (rR(LR) - TEST_PC));
	fail_if(8 != (rR(PC) - TEST_PC));

	cc.blx(0x43);
	run_test();
	fail_if(0x42 != rR(PC));
	fail_if(!ARM_CPSR_BEXT(Thumb));

	return(1);
}
