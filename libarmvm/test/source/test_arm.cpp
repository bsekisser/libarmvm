#include "test_arm.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

int test_arm::run_tests(void)
{
	LOG_ACTION(adcs());
	LOG_ACTION(add());
	LOG_ACTION(adds());
	LOG_ACTION(ands());
	LOG_ACTION(b_bl());
	LOG_ACTION(bics());
	LOG_ACTION(movs());
	LOG_ACTION(subs());
	LOG_ACTION(rsbs());
	LOG_ACTION(shift());
	return(1);
}
