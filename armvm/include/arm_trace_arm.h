#pragma once

#include "arm_trace.h"

/* **** */

void arm_trace_b_bl(arm_trace_p atp);
void arm_trace_dp(arm_trace_p atp);
void arm_trace_ldst(arm_trace_p atp);
void arm_trace_mla(arm_trace_p atp);
void arm_trace_msr(arm_trace_p atp);
void arm_trace_umull(arm_trace_p atp);
