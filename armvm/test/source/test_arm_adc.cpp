extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_adcs(test_p t)
{
	reset(t);

	rR(1) = ~0; rR(2) = 1;
	adcs(r0, r1, r2);
	run_test(t);
	fail_if(0 != rR(0));
	check_nzcv(t, 0, 1, 1, 0);

	rR(1) = 12; rR(2) = 1;
	adcs(r0, r1, r2);
	run_test(t);
	fail_if(13 != rR(0));
	check_nzcv(t, 0, 0, 0, 0);

	rR(1) = -0; rR(2) = -1;
	adcs(r0, r1, r2);
	run_test(t);
	fail_if(-1U != rR(0));
	check_nzcv(t, 1, 0, 0, 0);

	rR(1) = -1; rR(2) = -1;
	adcs(r0, r1, r2);
	run_test(t);
	fail_if(-2U != rR(0));
	check_nzcv(t, 1, 0, 1, 0);

	rR(1) = _BV(31); rR(2) = _BV(31);
	adcs(r0, r1, r2);
	run_test(t);
	fail_if(0 != rR(0));
	check_nzcv(t, 0, 1, 1, 1);

	return(1);
}}
