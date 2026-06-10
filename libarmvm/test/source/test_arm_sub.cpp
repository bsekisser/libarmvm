#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::subs(void)
{
	reset();

	vR(1) = 13; vR(2) = 12;
	cc.subs(rR0, rR1, rR2);
	run_test();
	fail_if(1 != vR(0));
	check_nzcv(0, 0, 1, 0);

	vR(1) = 12; vR(2) = 13;
	cc.subs(rR0, rR1, rR2);
	run_test();
	fail_if(-1U != vR(0));
	check_nzcv(1, 0, 0, 0);

	vR(1) = 0x1c; vR(2) = 0x1c;
	cc.subs(rR0, rR1, rR2);
	run_test();
	fail_if(0 != vR(0));
	check_nzcv(0, 1, 1, 0);

	vR(1) = 0x1d; vR(2) = 0x1c;
	cc.subs(rR0, rR1, rR2);
	run_test();
	fail_if(1 != vR(0));
	check_nzcv(0, 0, 1, 0);

	vR(0) = 0x1f; vR(1) = 0x09;
	cc.subs(rR1, rR1, cc.lsr(rR0, 4));
	run_test();
	fail_if(0x00000008 != vR(1));
	check_nzcv(0, 0, 1, 0);

	vR(1) = 64 << 24; vR(2) = 65 << 24;
	cc.subs(rR0, rR1, rR2);
	run_test();
	fail_if((0xffU << 24) != vR(0));
//	check_nzcv(1, 0, 1, 1); // !
	check_nzcv(1, 0, 0, 0); // ?

	vR(0) = 0; vR(1) = 0; vR(2) = 1;
	cc.subs(rR1, rR1, rR2);
	run_test();
	fail_if(~0U != vR(1));
	check_nzcv(1, 0, 0, 0);

		cc.sbcs(rR0, rR0, rR0);
		step_test();
		fail_if(~0U != vR(0));
		check_nzcv(1, 0, 0, 0);

	vR(1) = 0x60000000; vR(2) = 0x20000000;
	cc.subs(rR0, rR1, rR2);
	run_test_flags(_BV(ARM_CPSR(C)));
	fail_if(0x40000000 != vR(0));
	check_nzcv(0, 0, 1, 0);

	vR(1) = 0x60000000; vR(2) = 0xc0000000;
	cc.subs(rR0, rR1, rR2);
	run_test_flags(_BV(ARM_CPSR(C)));
	fail_if(0xa0000000 != vR(0));
	check_nzcv(1, 0, 0, 1);

	return(0);
}
