#pragma once

#include "armvm_trace.h"

/* **** */

void armvm_trace_b_bl(armvm_trace_p const atp);
void armvm_trace_blx(armvm_trace_p const atp);
void armvm_trace_bx_m(armvm_trace_p const atp);
void armvm_trace_blx_m(armvm_trace_p const atp);
void armvm_trace_dp(armvm_trace_p const atp);
void armvm_trace_ldst(armvm_trace_p const atp);
void armvm_trace_mcr_mrc(armvm_trace_p const atp);
void armvm_trace_mla(armvm_trace_p const atp);
void armvm_trace_mrs(armvm_trace_p const atp);
void armvm_trace_msr(armvm_trace_p const atp);
void armvm_trace_umull(armvm_trace_p const atp);
