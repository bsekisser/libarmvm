#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::add(void)
{
	reset();

	rR(0) = 0;
	cc.add(r0, r0, 1);
	run_test();
	fail_if(1 != rR(0));

	cc.add(r0, r0, cc.lsl(r0, 1));
	run_test();
	fail_if(3 != rR(0));

	cc.add(rPC, rPC, 0);
	run_test();
	fail_if(8 != (rR(PC) - TEST_PC));

	cc.add(r0, rPC, 0);
	run_test();
	fail_if(8 != (rR(0) - TEST_PC));

	return(1);
}

int test_arm::adds(void)
{
	reset();

	rR(0) = 1;
	cc.adds(r0, r0, cc.lsl(r0, 3));
	run_test();
	fail_if(0x00000009 != rR(0));
	check_nzcv(0, 0, 0, 0);

	rR(1) = 0x7fffffff; rR(2) = 0x7fffffff;
	cc.adds(r0, r1, r2);
	run_test();
	fail_if(0xfffffffe != rR(0));
	check_nzcv(1, 0, 0, 1);

	rR(1) = 0xffffffff; rR(2) = 0x00000001;
	cc.adds(r0, r1, r2);
	run_test();
	fail_if(0x00000000 != rR(0));
	check_nzcv(0, 1, 1, 0);

	rR(1) = 0x60000000; rR(2) = 0x20000000;
	cc.adds(r0, r1, r2);
	run_test();
	fail_if(0x80000000 != rR(0));
	check_nzcv(1, 0, 0, 1);

	rR(1) = 0x60000000; rR(2) = 0xc0000000;
	cc.adds(r0, r1, r2);
	run_test();
	fail_if(0x20000000 != rR(0));
	check_nzcv(0, 0, 1, 0);

	return(1);
}
