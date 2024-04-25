#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include <stdint.h>

/* **** */

#ifndef rR_IS_PC
	#define rR_IS_PC(_x) (ARMVM_GPR(PC) == ARM_IR_R(_x))
#endif

#ifndef rR_IS_NOT_PC
	#define rR_IS_NOT_PC(_x) (ARMVM_GPR(PC) != ARM_IR_R(_x))
#endif

/* **** */

static void setup_rR(armvm_core_p const core, const unsigned rrx, const unsigned r)
{ rRx(rrx) = r; vRx(rrx) = 0xfeedface; }

static uint32_t setup_rR_vR(armvm_core_p const core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
	rRx(rrx) = r;
	vRx(rrx) = v;

	return(v);
}

UNUSED_FN
static void setup_vR(armvm_core_p const core, const unsigned rrx, const uint32_t v)
{ rRx(rrx) = ~0U; vRx(rrx) = v; }

/* **** */

static void core_reg_dst(armvm_core_p const core, const unsigned rrx,
	const unsigned r)
{ setup_rR(core, rrx, r & 15); }

UNUSED_FN
static void core_reg_dst_decode(armvm_core_p const core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ return(core_reg_dst(core, rrx, mlBFEXT(IR, msb, lsb))); }

static uint32_t core_reg_dst_wb(armvm_core_p const core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
	const unsigned rr = r & 15;

	if(CCX) GPRx(rr) = v;

	return(setup_rR_vR(core, rrx, rr, v));
}

UNUSED_FN
static void core_reg_wb(armvm_core_p const core, const unsigned rrx)
{ if(CCX) GPRx(rRx(rrx)) = vRx(rrx); }

static uint32_t core_reg_src(armvm_core_p const core, const unsigned rrx,
	const unsigned r)
{
	const unsigned rr = r & 15;
	uint32_t v = GPRx(rr);

	if(ARMVM_GPR(PC) == r)
		v += (4 >> IF_CPSR(Thumb));

	return(setup_rR_vR(core, rrx, rr, v));
}

UNUSED_FN
static uint32_t core_reg_src_decode(armvm_core_p const core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ return(core_reg_src(core, rrx, mlBFEXT(IR, msb, lsb))); }
