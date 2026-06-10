#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::adcs(void)
{
	reset();

	vR(1) = ~0; vR(2) = 1;
	cc.adcs(rR0, rR1, rR2);
	run_test();
	fail_if(0 != vR(0));
	check_nzcv(0, 1, 1, 0);

	vR(1) = 12; vR(2) = 1;
	cc.adcs(rR0, rR1, rR2);
	run_test();
	fail_if(13 != vR(0));
	check_nzcv(0, 0, 0, 0);

	vR(1) = -0; vR(2) = -1;
	cc.adcs(rR0, rR1, rR2);
	run_test();
	fail_if(-1U != vR(0));
	check_nzcv(1, 0, 0, 0);

	vR(1) = -1; vR(2) = -1;
	cc.adcs(rR0, rR1, rR2);
	run_test();
	fail_if(-2U != vR(0));
	check_nzcv(1, 0, 1, 0);

	vR(1) = _BV(31); vR(2) = _BV(31);
	cc.adcs(rR0, rR1, rR2);
	run_test();
	fail_if(0 != vR(0));
	check_nzcv(0, 1, 1, 1);

	vR(0) = 0; vR(2) = ~0; vR(3) = 1;
	cc.adds(rR1, rR2, rR3);
	cc.adcs(rR0, rR0, rR0);
	run_test();
	fail_if(0 != vR(1));
	fail_if(1 != vR(0));
	check_nzcv(0, 0, 0, 0);


	return(1);
}
