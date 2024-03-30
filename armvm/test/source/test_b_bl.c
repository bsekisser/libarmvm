#include "test_utility.h"
#include "test.h"

/* **** */

int test_b_bl(test_p t)
{
	unsigned test_pc = 0;

	reset(t);

	test_pc = b(0);
	test(t);
	fail_if(8 != (rR(PC) - test_pc));

	test_pc = bl(0);
	test(t);
	fail_if(8 != (rR(PC) - test_pc));
	fail_if(4 != (rR(LR) - test_pc));

	return(1);
}
