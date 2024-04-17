#pragma once

/* **** */

#include <stdint.h>

/* **** */

typedef void (*__alubox_fn)(armvm_core_p const core, const unsigned* rRD,
	const unsigned* rRN, const uint32_t rm);

static void __alubox_wb(armvm_core_p const core, const unsigned* rRD,
	const unsigned* rRN, const uint32_t rm, __alubox_fn fn);
{
	const uint32_t rd = fn(core, rRN, rm);
	if(rRD) arm_reg_dst_wb(core, ARMVM_TRACE_R(D), rRD, rd);
}

/* **** */

static uint32_t __alubox__add(armvm_core_p const core, const unsigned* rRN, const uint32_t rm)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), rRN);

	return(rn + rm);
}

static uint32_t __alubox__mov(armvm_core_p const core, const unsigned* rRN, const uint32_t rm)
{ return(rm); UNUSED(core, rRN); }
