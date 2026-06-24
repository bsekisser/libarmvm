#pragma once

/* **** */

#define _APSR(_) (apsr._)
#define _APSRp(_) (apsr->_)

#define APSR(_) (_APSR(in)._)
#define APSRp(_) (_APSRp(in)._)

#define APSR_OUT(_) (_APSR(out)._)
#define APSRp_OUT(_) (_APSRp(out)._)

#define APSR_BEXT(_v, _bit) (((_v) >> ARM_APSR_BIT(_bit)) & 1)
#define APSR_BSET_AS(_bit, _as) ((_as) << ARM_APSR_BIT(_bit))
