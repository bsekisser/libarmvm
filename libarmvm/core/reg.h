#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include "libarm/include/cpsr.h"

/* **** */

#include <stdint.h>

/* **** */

#ifndef rR_IS_PC
	#define rR_IS_PC(_x) (rPC == ARM_IR_R(_x))
#endif

#ifndef rR_IS_NOT_PC
	#define rR_IS_NOT_PC(_x) (rPC != ARM_IR_R(_x))
#endif

/* **** */

static uint32_t __reg_src_fetch(armvm_core_ref core, const unsigned rrx, const unsigned r)
{
	const unsigned rr = r & 15;
	uint32_t v = GPRx(rr);

	if(rPC == r) {
		const unsigned thumb = IF_CPSR(Thumb);
		v += (4 >> thumb);
		v &= ~(3U >> thumb);
	}

	vRx(rrx) = v;

	return(v);
}

static void __reg_wb(armvm_core_ref core, const unsigned rrx, const uint32_t v)
{
	vRx(rrx) = v;

	if(CCX) GPRx(rRx(rrx)) = v;
}

static uint32_t __reg_setup_rR_vR(armvm_core_ref core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
	rRx(rrx) = r;
	vRx(rrx) = v;

	return(v);
}

/* **** */

static void _reg_setup_rR(armvm_core_ref core, const unsigned rrx, const unsigned r)
{ (void)__reg_setup_rR_vR(core, rrx, r, 0xfeedface); }

static uint32_t _reg_setup_rR_src(armvm_core_ref core, const unsigned rrx, const unsigned r)
{
	_reg_setup_rR(core, rrx, r & 15);
	return(__reg_src_fetch(core, rrx, r & 15));
}

static void _reg_setup_rRml(armvm_core_ref core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ _reg_setup_rR(core, rrx, mlBFEXT(IR, msb, lsb)); }

UNUSED_FN
static uint32_t _reg_setup_vR(armvm_core_ref core, const unsigned rrx, const uint32_t v)
{ return(__reg_setup_rR_vR(core, rrx, ~0U, v)); }

/* **** */

UNUSED_FN
static void reg_dst(armvm_core_ref core, const unsigned rrx,
	const unsigned r)
{ _reg_setup_rR(core, rrx, r & 15); }

UNUSED_FN
static void reg_dst_decode(armvm_core_ref core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ _reg_setup_rRml(core, rrx, msb, lsb); }

static uint32_t reg_dst_wb(armvm_core_ref core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{
	_reg_setup_rR(core, rrx, r & 15);
	__reg_wb(core, rrx, v);

	return(v);
}

UNUSED_FN
static void reg_src_setup(armvm_core_ref core, const unsigned rrx,
	const unsigned r)
{ _reg_setup_rR(core, rrx, r & 15); }

static uint32_t reg_src(armvm_core_ref core, const unsigned rrx,
	const unsigned r)
{ return(_reg_setup_rR_src(core, rrx, r)); }

UNUSED_FN
static uint32_t reg_src_decode(armvm_core_ref core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ return(_reg_setup_rR_src(core, rrx, mlBFEXT(IR, msb, lsb))); }

UNUSED_FN
static uint32_t reg_src_load(armvm_core_ref core, const unsigned rrx)
{ return(__reg_src_fetch(core, rrx, rRx(rrx))); }

UNUSED_FN
static void reg_wb(armvm_core_ref core, const unsigned rrx)
{ __reg_wb(core, rrx, vRx(rrx)); }

UNUSED_FN
static void reg_wb_v(armvm_core_ref core, const unsigned rrx, const uint32_t v)
{ __reg_wb(core, rrx, v); }

/* **** */

UNUSED_FN
static void reg_setup_rR(armvm_core_ref core, const unsigned rrx, const unsigned r)
{ _reg_setup_rR(core, rrx, r); }

UNUSED_FN
static void reg_setup_rRml(armvm_core_ref core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ _reg_setup_rRml(core, rrx, msb, lsb); }

UNUSED_FN
static uint32_t reg_setup_rR_vR(armvm_core_ref core, const unsigned rrx,
	const unsigned r, const uint32_t v)
{ return(__reg_setup_rR_vR(core, rrx, r, v)); }

UNUSED_FN
static uint32_t reg_setup_vR(armvm_core_ref core, const unsigned rrx, const uint32_t v)
{ return(_reg_setup_vR(core, rrx, v)); }

UNUSED_FN
static uint32_t reg_setup_vRml(armvm_core_ref core, const unsigned rrx,
	const unsigned msb, const unsigned lsb)
{ return(reg_setup_vR(core, rrx, mlBFEXT(IR, msb, lsb))); }
