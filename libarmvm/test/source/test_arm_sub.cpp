extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_subs(test_p t)
{
	reset(t);

	rR(1) = 13; rR(2) = 12;
	subs(r0, r1, r2);
	run_test(t);
	fail_if(1 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);

	rR(1) = 12; rR(2) = 13;
	subs(r0, r1, r2);
	run_test(t);
	fail_if(-1U != rR(0));
	check_nzcv(t, 1, 0, 0, 0);

	rR(1) = 0x1c; rR(2) = 0x1c;
	subs(r0, r1, r2);
	run_test(t);
	fail_if(0 != rR(0));
	check_nzcv(t, 0, 1, 1, 0);

	rR(1) = 0x1d; rR(2) = 0x1c;
	subs(r0, r1, r2);
	run_test(t);
	fail_if(1 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);

	rR(0) = 0x1f; rR(1) = 0x09;
	subs(r1, r1, LSR(r0, 4));
	run_test(t);
	fail_if(0x00000008 != rR(1));
	check_nzcv(t, 0, 0, 1, 0);

	rR(1) = 64 << 24; rR(2) = 65 << 24;
	subs(r0, r1, r2);
	run_test(t);
	fail_if((0xffU << 24) != rR(0));
//	check_nzcv(t, 1, 0, 1, 1); // !
	check_nzcv(t, 1, 0, 0, 0); // ?

	rR(0) = 0; rR(1) = 0; rR(2) = 1;
	subs(r1, r1, r2);
	run_test(t);
	fail_if(~0U != rR(1));
	check_nzcv(t, 1, 0, 0, 0);

		sbcs(r0, r0, r0);
		step_test(t);
		fail_if(~0U != rR(0));
		check_nzcv(t, 1, 0, 0, 0);

	rR(1) = 0x60000000; rR(2) = 0x20000000;
	subs(r0, r1, r2);
	run_test_flags(t, _BV(ARM_CPSR(C)));
	fail_if(0x40000000 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);

	rR(1) = 0x60000000; rR(2) = 0xc0000000;
	subs(r0, r1, r2);
	run_test_flags(t, _BV(ARM_CPSR(C)));
	fail_if(0xa0000000 != rR(0));
	check_nzcv(t, 1, 0, 0, 1);

	return(1);
}}
