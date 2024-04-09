extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_movs(test_p t)
{
	reset(t);

	movs(r0, 0);
	run_test(t);
	fail_if(0x00000000 != rR(0));
	check_nz(t, 0, 1);

	rR(1) = ~0;
	movs(r0, r1);
	run_test(t);
	fail_if(~0U != rR(0));
	check_nz(t, 1, 0);

	movs(r0, ROR(64, 0));
	run_test(t);
	fail_if(0x00000040 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(64, 26));
	run_test(t);
	fail_if(0x00001000 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(64, 28));
	run_test(t);
	fail_if(0x00000400 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(0x3f, 0x1c));
	run_test(t);
	fail_if(0x000003f0 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(0xfc, 0x1e));
	run_test(t);
	fail_if(0x000003f0 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(0x80, 0));
	run_test(t);
	fail_if(0x00000080 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(0x80, 2));
	run_test(t);
	fail_if(0x00000020 != rR(0));
	check_nz(t, 0, 0);

	movs(r0, ROR(0x80, 8));
	run_test(t);
	fail_if(0x80000000 != rR(0));
	check_nzc(t, 1, 0, 1);

	return(1);
}}
