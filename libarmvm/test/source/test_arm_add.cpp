#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::add(void)
{
	reset();

	vR(0) = 0;
	cc.add(rR0, rR0, 1);
	run_test();
	fail_if(1 != vR(0));

	cc.add(rR0, rR0, cc.lsl(rR0, 1));
	run_test();
	fail_if(3 != vR(0));

	cc.add(rPC, rPC, 0);
	run_test();
	fail_if(8 != (PC - TEST_PC));

	cc.add(rR0, rPC, 0);
	run_test();
	fail_if(8 != (vR(0) - TEST_PC));

	return(1);
}

int test_arm::adds(void)
{
	reset();

	vR(0) = 1;
	cc.adds(rR0, rR0, cc.lsl(rR0, 3));
	run_test();
	fail_if(0x00000009 != vR(0));
	check_nzcv(0, 0, 0, 0);

	vR(1) = 0x7fffffff; vR(2) = 0x7fffffff;
	cc.adds(rR0, rR1, rR2);
	run_test();
	fail_if(0xfffffffe != vR(0));
	check_nzcv(1, 0, 0, 1);

	vR(1) = 0xffffffff; vR(2) = 0x00000001;
	cc.adds(rR0, rR1, rR2);
	run_test();
	fail_if(0x00000000 != vR(0));
	check_nzcv(0, 1, 1, 0);

	vR(1) = 0x60000000; vR(2) = 0x20000000;
	cc.adds(rR0, rR1, rR2);
	run_test();
	fail_if(0x80000000 != vR(0));
	check_nzcv(1, 0, 0, 1);

	vR(1) = 0x60000000; vR(2) = 0xc0000000;
	cc.adds(rR0, rR1, rR2);
	run_test();
	fail_if(0x20000000 != vR(0));
	check_nzcv(0, 0, 1, 0);

	return(1);
}
