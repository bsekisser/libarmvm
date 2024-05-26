#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::adcs(void)
{
	reset();

	rR(1) = ~0; rR(2) = 1;
	cc.adcs(r0, r1, r2);
	run_test();
	fail_if(0 != rR(0));
	check_nzcv(0, 1, 1, 0);

	rR(1) = 12; rR(2) = 1;
	cc.adcs(r0, r1, r2);
	run_test();
	fail_if(13 != rR(0));
	check_nzcv(0, 0, 0, 0);

	rR(1) = -0; rR(2) = -1;
	cc.adcs(r0, r1, r2);
	run_test();
	fail_if(-1U != rR(0));
	check_nzcv(1, 0, 0, 0);

	rR(1) = -1; rR(2) = -1;
	cc.adcs(r0, r1, r2);
	run_test();
	fail_if(-2U != rR(0));
	check_nzcv(1, 0, 1, 0);

	rR(1) = _BV(31); rR(2) = _BV(31);
	cc.adcs(r0, r1, r2);
	run_test();
	fail_if(0 != rR(0));
	check_nzcv(0, 1, 1, 1);

	rR(0) = 0; rR(2) = ~0; rR(3) = 1;
	cc.adds(r1, r2, r3);
	cc.adcs(r0, r0, r0);
	run_test();
	fail_if(0 != rR(1));
	fail_if(1 != rR(0));
	check_nzcv(0, 0, 0, 0);


	return(1);
}
