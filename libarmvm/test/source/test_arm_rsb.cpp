#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::rsbs(void)
{
	reset();

	rR(0) = 1;
	cc.rsbs(r0, r0, cc.lsl(r0, 3));
	run_test();
	fail_if(0x00000007 != rR(0));
	check_nzcv(0, 0, 1, 0);

	return(0);
}
