#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::bics(void)
{
	reset();

	vR(1) = 0xffffffff; vR(2) = 0x0f0f0f0f;
	cc.bics(rR0, rR1, rR2);
	run_test();
	fail_if(0xf0f0f0f0 != vR(0));
	check_nz(1, 0);

	vR(1) = 0xffffffff; vR(2) = 0xf0f0f0f0;
	cc.bics(rR0, rR1, rR2);
	run_test();
	fail_if(0x0f0f0f0f != vR(0));
	check_nz(0, 0);

	vR(1) = 0x0f0f0f0f; vR(2) = 0xf0f0f0f0;
	cc.bics(rR0, rR1, rR2);
	run_test();
	fail_if(0x0f0f0f0f != vR(0));
	check_nz(0, 0);

	vR(1) = 0x0f0f0f0f; vR(2) = 0x0f0f0f0f;
	cc.bics(rR0, rR1, rR2);
	run_test();
	fail_if(0 != vR(0));
	check_nz(0, 1);

	return(1);
}
