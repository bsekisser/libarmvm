#pragma once

/* **** */

#include "armvm_core_glue.h"

/* **** */

#include "armvm.h"

/* **** */

#include "git/libbse/include/log.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t _setup_rR_vR(armvm_core_p core, unsigned rrx, unsigned rr, uint32_t vr)
{
//	LOG("rrx: 0x%02x, rr: 0x%02x, vr: 0x%08x", rrx, rr, vr);

	if(pARMVM_TRACE && (~0 != rrx)) {
		rRx(rrx) = rr;
		vRx(rrx) = vr;
	}

	return(vr);
}

/*static inline void arm_reg_dst(armvm_core_p core, unsigned rrx, unsigned rdst)
{
	_setup_rR_vR(core, rrx, rdst, GPRx(rdst));
}*/

static inline void arm_reg_dst_wb(armvm_core_p core, unsigned rrx, unsigned rdst, uint32_t data)
{
	if(CCX) GPRx(rdst) = data;

	_setup_rR_vR(core, rrx, rdst, data);
}

static inline uint32_t arm_reg_src(armvm_core_p core, unsigned rrx, unsigned rsrc)
{
	uint32_t vr = GPRx(rsrc);

	if(ARMVM_GPR(PC) == rsrc)
		vr += 4;

	return(_setup_rR_vR(core, rrx, rsrc, vr));
}

/* **** */

static inline uint32_t setup_rR_vR(armvm_core_p core, unsigned rrx, unsigned rr, uint32_t vr)
{
	return(_setup_rR_vR(core, rrx, rr, vr));
}

