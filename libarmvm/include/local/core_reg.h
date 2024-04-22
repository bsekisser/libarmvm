#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include <stdint.h>

/* **** */

//#define rRx(_x) core->armvm_trace.vr[_x]
//#define rR(_x) rRx(ARMVM_TRACE_R(_x))

//#define vRx(_x) core->armvm_trace.vr[_x]
//#define vR(_x) vRx(ARMVM_TRACE_R(_x))

#ifndef rR_IS_PC
	#define rR_IS_PC(_x) (ARMVM_GPR(PC) == ARM_IR_R(_x))
#endif

#ifndef rR_IS_NOT_PC
	#define rR_IS_NOT_PC(_x) (ARMVM_GPR(PC) != ARM_IR_R(_x))
#endif

/* **** */

static uint32_t setup_rR_vR(armvm_core_p const core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
//	if(core->config.trace) {
		rRx(rrx) = r;
		vRx(rrx) = v;
//	}

	return(v);
}

static uint32_t core_reg_dst_wb(armvm_core_p const core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
	const unsigned rr = r & 15;

	if(CCX) GPRx(rr) = v;

	return(setup_rR_vR(core, rrx, rr, v));
}

static uint32_t core_reg_src(armvm_core_p const core, const unsigned rrx,
	const unsigned r)
{
	const unsigned rr = r & 15;
//	const uint32_t v = GPRx(rr) + ((!!(ARMVM_GPR(PC) == r)) << 2);
	uint32_t v = GPRx(rr);

	if(ARMVM_GPR(PC) == r)
		v += (4 >> IF_CPSR(Thumb));

	return(setup_rR_vR(core, rrx, rr, v));
}
