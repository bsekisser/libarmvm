#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::shift()
{
	reset();

	rR(1) = 1;
	cc.asrs(r0, r1, 0);
	run_test();
	fail_if(0 != rR(0));
	check_nzc(0, 1, 0);

	rR(1) = 0x40000000;
	cc.asrs(r0, r1, 0);
	run_test();
	fail_if(0 != rR(0));
	check_nzc(0, 1, 0);

	rR(1) = 0x80000000;
	cc.asrs(r0, r1, 31);
	run_test();
	fail_if(-1U != rR(0));
	check_nzc(1, 0, 0);

	rR(1) = 0x80000000;
	cc.asrs(r0, r1, 0);
	run_test();
	fail_if(-1U != rR(0));
	check_nzc(1, 0, 1);

	cc.movs(r1, cc.ror(0x80, 8));
	cc.asrs(r1, r1, 0);
	cc.lsls(r1, r1, 16);
	cc.lsrs(r1, r1, 8);
	run_test();
	fail_if(0x00ffff00 != rR(1));
	check_nzc(0, 0, 0);

	cc.movs(r1, cc.ror(1, 16));
	run_test();
	fail_if(0x00010000 != rR(1));
	check_nzc(0, 0, 0);

	cc.movs(r1, cc.ror(1, 30));
	run_test();
	fail_if(0x00000004 != rR(1));
	check_nzc(0, 0, 0);

	rR(1) = 0x00000005;
	cc.lsls(r0, r1, 29);
	run_test();
	fail_if(0xa0000000 != rR(0));
	check_nzc(1, 0, 0);

		cc.lsls(r0, r1, 30);
		step_test();
		fail_if(0x40000000 != rR(0));
		check_nzc(0, 0, 1);

		cc.lsls(r0, r1, 31);
		step_test();
		fail_if(0x80000000 != rR(0));
		check_nzc(1, 0, 0);

	rR(1) = 0xffffffe6;
	cc.lsls(r0, r1, 30);
	run_test();
	fail_if(0x80000000 != rR(0));
	check_nzc(1, 0, 1);

		cc.lsls(r0, r1, 31);
		step_test();
		fail_if(0x00000000 != rR(0));
		check_nzc(0, 1, 1);

		cc.lsls(r1, r1, 31);
		step_test();
		fail_if(0x00000000 != rR(1));
		check_nzc(0, 1, 1);

	return(1);
}
