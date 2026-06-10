#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::shift()
{
	reset();

	vR(1) = 1;
	cc.asrs(rR0, rR1, 0);
	run_test();
	fail_if(0 != vR(0));
	check_nzc(0, 1, 0);

	vR(1) = 0x40000000;
	cc.asrs(rR0, rR1, 0);
	run_test();
	fail_if(0 != vR(0));
	check_nzc(0, 1, 0);

	vR(1) = 0x80000000;
	cc.asrs(rR0, rR1, 31);
	run_test();
	fail_if(-1U != vR(0));
	check_nzc(1, 0, 0);

	vR(1) = 0x80000000;
	cc.asrs(rR0, rR1, 0);
	run_test();
	fail_if(-1U != vR(0));
	check_nzc(1, 0, 1);

	cc.movs(rR1, cc.ror(0x80, 8));
	cc.asrs(rR1, rR1, 0);
	cc.lsls(rR1, rR1, 16);
	cc.lsrs(rR1, rR1, 8);
	run_test();
	fail_if(0x00ffff00 != vR(1));
	check_nzc(0, 0, 0);

	cc.movs(rR1, cc.ror(1, 16));
	run_test();
	fail_if(0x00010000 != vR(1));
	check_nzc(0, 0, 0);

	cc.movs(rR1, cc.ror(1, 30));
	run_test();
	fail_if(0x00000004 != vR(1));
	check_nzc(0, 0, 0);

	vR(1) = 0x00000005;
	cc.lsls(rR0, rR1, 29);
	run_test();
	fail_if(0xa0000000 != vR(0));
	check_nzc(1, 0, 0);

		cc.lsls(rR0, rR1, 30);
		step_test();
		fail_if(0x40000000 != vR(0));
		check_nzc(0, 0, 1);

		cc.lsls(rR0, rR1, 31);
		step_test();
		fail_if(0x80000000 != vR(0));
		check_nzc(1, 0, 0);

	vR(1) = 0xffffffe6;
	cc.lsls(rR0, rR1, 30);
	run_test();
	fail_if(0x80000000 != vR(0));
	check_nzc(1, 0, 1);

		cc.lsls(rR0, rR1, 31);
		step_test();
		fail_if(0x00000000 != vR(0));
		check_nzc(0, 1, 1);

		cc.lsls(rR1, rR1, 31);
		step_test();
		fail_if(0x00000000 != vR(1));
		check_nzc(0, 1, 1);

	return(1);
}
