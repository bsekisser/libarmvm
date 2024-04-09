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

	return(1);
}}
