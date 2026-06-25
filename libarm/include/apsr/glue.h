#pragma once

/* **** */

#define _APSR(_) (apsr._)
#define _APSRp(_) (apsr->_)

#define APSR(_) (_APSR(in)._)
#define APSRp(_) (_APSRp(in)._)

#define APSRp_FLAGS(_) (_APSRp(flags)._)

#define APSR_OUT(_) (_APSR(out)._)
//#define APSRp_OUT(_) (_APSRp(out)._)
//#define APSRp_OUT_SET_IF(_, _v) ({ APSRp_OUT(_) = APSRp_SET_IF(_, _v); })

#define APSR_SET(_) (_APSR(set)._)
#define APSRp_SET(_) (_APSRp(set)._)

//#define APSRp_SET_IF(_, _v) ({ typeof(_v) __v = _v; ({ if(APSRp_FLAGS(set)) APSRp_SET(_) = _v; }) __v; })

#define APSR_BEXT(_v, _bit) (((_v) >> ARM_APSR_BIT(_bit)) & 1)
#define APSR_BSET_AS(_bit, _as) ((_as) << ARM_APSR_BIT(_bit))
