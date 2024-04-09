#include "test_arm.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

void test_arm(test_p t)
{
	LOG_ACTION(test_arm_adcs(t));
	LOG_ACTION(test_arm_add(t));
	LOG_ACTION(test_arm_adds(t));
	LOG_ACTION(test_arm_ands(t));
	LOG_ACTION(test_arm_b_bl(t));
	LOG_ACTION(test_arm_bics(t));
	LOG_ACTION(test_arm_movs(t));
	LOG_ACTION(test_arm_subs(t));
	LOG_ACTION(test_arm_rsbs(t));
	LOG_ACTION(test_arm_shift(t));
}
