#include "apsr/condition_code.h"

/* **** */

#include "apsr/apsr.h"
#include "apsr/glue.h"
#include "cc.h"

/* **** */

#include "libbse/include/log.h"

#include <stdlib.h>

/* **** */

#define NeqV (APSR(n) == APSR(v))

__attribute__((visibility("default")))
unsigned _arm_condition_check(arm_apsr_tref apsr, arm_condition_eref cc)
{
	switch(cc) {
		case CC_AL: return(1);
		case CC_CC: return(!APSR(c));
		case CC_CS: return(APSR(c));
		case CC_EQ: return(APSR(z));
		case CC_GE: return(NeqV);
		case CC_GT: return(!APSR(z) && NeqV);
		case CC_HI: return(APSR(c) && !APSR(z));
		case CC_LE: return(APSR(z) || !NeqV);
		case CC_LS: return(!APSR(c) || APSR(z));
		case CC_LT: return(!NeqV);
		case CC_MI: return(APSR(n));
		case CC_NE: return(!APSR(z));
		case CC_NV: break;
		case CC_PL: return(!APSR(n));
		case CC_VC: return(!APSR(v));
		case CC_VS: return(APSR(v));
		default: LOG_ACTION(exit(-1));
	}

	return(0);
}

__attribute__((visibility("default")))
unsigned arm_condition_check(arm_apsr_tref apsr, arm_condition_eref cc)
{
	const unsigned result = _arm_condition_check(apsr, cc);
	return(result ? ~cc : cc);
}
