extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_rsbs(test_p t)
{
	reset(t);

	rR(0) = 1;
	rsbs(r0, r0, LSL(r0, 3));
	run_test(t);
	fail_if(0x00000007 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);

	return(0);
}}
