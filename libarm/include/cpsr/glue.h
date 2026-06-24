#pragma once

/* **** */

#define CPSR(_) (cpsr._)
#define CPSRp(_) (cpsr->_)

#define CPSR_BEXT(_v, _bit) (((_v) >> ARM_CPSR_BIT(_bit)) & 1)
#define CPSR_BSET_AS(_bit, _as) ((_as) << ARM_CPSR_BIT(_bit))
