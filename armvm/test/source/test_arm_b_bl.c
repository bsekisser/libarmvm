#include "test_utility.h"
#include "test.h"

/* **** */

#include "test_arm_gen.h"

/* **** */

#include "libarmvm/include/armvm.h"

/* **** */

#include <stdint.h>

/* **** */

int test_b_bl(test_p t)
{
	armvm_reset(pARMVM);

	b(0);
	run_test(t);
	fail_if(8 != (rR(PC) - TEST_PC));

	bl(0);
	run_test(t);
	fail_if(8 != (rR(PC) - TEST_PC));
	fail_if(4 != (rR(LR) - TEST_PC));

	return(1);
}
