#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::ands(void)
{
	reset();

	rR(1) = 0xf0f0f0f0; rR(2) = 0x0f0f0f0f;
	cc.ands(r0, r1, r2);
	run_test();
	fail_if(0 != rRx(0));
	check_nz(0, 1);

	rR(1) = 0xffffffff; rR(2) = 0x0f0f0f0f;
	cc.ands(r0, r1, r2);
	run_test();
	fail_if(0x0f0f0f0f != rRx(0));
	check_nz(0, 0);

	rR(1) = 0xffffffff; rR(2) = 0xf0f0f0f0;
	cc.ands(r0, r1, r2);
	run_test();
	fail_if(0xf0f0f0f0 != rRx(0));
	check_nz(1, 0);

	return(1);
}
