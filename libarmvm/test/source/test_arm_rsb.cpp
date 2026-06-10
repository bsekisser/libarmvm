#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::rsbs(void)
{
	reset();

	vR(0) = 1;
	cc.rsbs(rR0, rR0, cc.lsl(rR0, 3));
	run_test();
	fail_if(0x00000007 != vR(0));
	check_nzcv(0, 0, 1, 0);

	return(0);
}
