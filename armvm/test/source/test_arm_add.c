#include "test_utility.h"
#include "test.h"

/* **** */

int test_add(test_p t)
{
	reset(t);

	add(r0, r0, 1);
	test(t);
	fail_if(0 != xR(0));

	add(r0, r0, LSL(1, 1));
	test(t);
	fail_if(3 != xR(0));

	const unsigned test_pc = add(r0, rPC, 0);
	test(t);
	fail_if(8 != (xR(0) - test_pc));

	return(1);
}
