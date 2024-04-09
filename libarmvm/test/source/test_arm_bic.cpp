extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_bics(test_p t)
{
	reset(t);

	rR(1) = 0xffffffff; rR(2) = 0x0f0f0f0f;
	bics(r0, r1, r2);
	run_test(t);
	fail_if(0xf0f0f0f0 != rRx(0));
	check_nz(t, 1, 0);

	rR(1) = 0xffffffff; rR(2) = 0xf0f0f0f0;
	bics(r0, r1, r2);
	run_test(t);
	fail_if(0x0f0f0f0f != rRx(0));
	check_nz(t, 0, 0);

	rR(1) = 0x0f0f0f0f; rR(2) = 0xf0f0f0f0;
	bics(r0, r1, r2);
	run_test(t);
	fail_if(0x0f0f0f0f != rRx(0));
	check_nz(t, 0, 0);

	rR(1) = 0x0f0f0f0f; rR(2) = 0x0f0f0f0f;
	bics(r0, r1, r2);
	run_test(t);
	fail_if(0 != rRx(0));
	check_nz(t, 0, 1);

	return(1);
}}
