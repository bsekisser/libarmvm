#pragma once

/* **** */

#include "apsr/apsr.h"
#include "cc.h"

/* **** */

__attribute__((warn_unused_result))
unsigned arm_condition_check(arm_apsr_tref apsr, arm_condition_eref cc);
