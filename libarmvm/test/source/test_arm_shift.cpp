extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_shift(test_p t)
{
	reset(t);

	rR(1) = 1;
	asrs(r0, r1, 0);
	run_test(t);
	fail_if(0 != rR(0));
	check_nzc(t, 0, 1, 0);

	rR(1) = 0x40000000;
	asrs(r0, r1, 0);
	run_test(t);
	fail_if(0 != rR(0));
	check_nzc(t, 0, 1, 0);

	rR(1) = 0x80000000;
	asrs(r0, r1, 31);
	run_test(t);
	fail_if(-1U != rR(0));
	check_nzc(t, 1, 0, 0);

	rR(1) = 0x80000000;
	asrs(r0, r1, 0);
	run_test(t);
	fail_if(-1U != rR(0));
	check_nzc(t, 1, 0, 1);

	movs(r1, ROR(0x80, 8));
	asrs(r1, r1, 0);
	lsls(r1, r1, 16);
	lsrs(r1, r1, 8);
	run_test(t);
	fail_if(0x00ffff00 != rR(1));
	check_nzc(t, 0, 0, 0);

	movs(r1, ROR(1, 16));
	run_test(t);
	fail_if(0x00010000 != rR(1));
	check_nzc(t, 0, 0, 0);

	movs(r1, ROR(1, 30));
	run_test(t);
	fail_if(0x00000004 != rR(1));
	check_nzc(t, 0, 0, 0);

	rR(1) = 0x00000005;
	lsls(r0, r1, 29);
	run_test(t);
	fail_if(0xa0000000 != rR(0));
	check_nzc(t, 1, 0, 0);

		lsls(r0, r1, 30);
		step_test(t);
		fail_if(0x40000000 != rR(0));
		check_nzc(t, 0, 0, 1);

		lsls(r0, r1, 31);
		step_test(t);
		fail_if(0x80000000 != rR(0));
		check_nzc(t, 1, 0, 0);

	rR(1) = 0xffffffe6;
	lsls(r0, r1, 30);
	run_test(t);
	fail_if(0x80000000 != rR(0));
	check_nzc(t, 1, 0, 1);

		lsls(r0, r1, 31);
		step_test(t);
		fail_if(0x00000000 != rR(0));
		check_nzc(t, 0, 1, 1);

		lsls(r1, r1, 31);
		step_test(t);
		fail_if(0x00000000 != rR(1));
		check_nzc(t, 0, 1, 1);

	return(1);
}}
