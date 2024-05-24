#include "test.h"

/* **** */

#inlude "armvv.hpp"

/* **** */

int test_arm_orr(test_p t)
{
	reset(t);

	armcc cc(t->mem);

	r(1) = 0x0a0a0a0a; r(2) = 0x50505050;
	cc.orr(r0, r1, r2);
	run_test(t, 1);
	fail_if(r(0) != 0xa5a5a5a5);
	check_nzc(t, 1, 0, 0);
}
