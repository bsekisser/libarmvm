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
	fail_if(0x10 != PC);

	cc.bl(0x20);
	run_test();
	fail_if(0x20 != PC);
	fail_if(4 != (LR - TEST_PC));

	const uint32_t bl_LR = LR;

	cc.blx(rLR);
	run_test();
	fail_if(bl_LR != PC);
	fail_if(4 != (LR - TEST_PC));
	fail_if(CPSR(thumb));

	const uint32_t blx_LR = LR;

	cc.bx(rLR);
	run_test();
	fail_if(blx_LR != PC);
	fail_if(CPSR(thumb));

	cc.bx(rPC);
	run_test();
	fail_if(8 != (PC - TEST_PC));

	cc.blx(rPC);
	run_test();
	fail_if(4 != (LR - TEST_PC));
	fail_if(8 != (PC - TEST_PC));

	cc.blx(0x43);
	run_test();
	fail_if(0x42 != PC);
	fail_if(!CPSR(thumb));

	return(1);
}
