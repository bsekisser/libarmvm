extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_add(test_p t)
{
	reset(t);

	rR(0) = 0;
	add(r0, r0, 1);
	run_test(t);
	fail_if(1 != rR(0));

	add(r0, r0, LSL(r0, 1));
	run_test(t);
	fail_if(3 != rR(0));

	add(r0, rPC, 0);
	run_test(t);
	fail_if(8 != (rR(0) - TEST_PC));

	return(1);
}

	int test_arm_adds(test_p t)
{
	reset(t);

	rR(0) = 1;
	adds(r0, r0, LSL(r0, 3));
	run_test(t);
	fail_if(0x00000009 != rR(0));
	check_nzcv(t, 0, 0, 0, 0);

	rR(1) = 0x7fffffff; rR(2) = 0x7fffffff;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0xfffffffe != rR(0));
	check_nzcv(t, 1, 0, 0, 1);

	rR(1) = 0xffffffff; rR(2) = 0x00000001;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x00000000 != rR(0));
	check_nzcv(t, 0, 1, 1, 0);

	rR(1) = 0x60000000; rR(2) = 0x20000000;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x80000000 != rR(0));
	check_nzcv(t, 1, 0, 0, 1);

	rR(1) = 0x60000000; rR(2) = 0xc0000000;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x20000000 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);

	return(1);
}}
