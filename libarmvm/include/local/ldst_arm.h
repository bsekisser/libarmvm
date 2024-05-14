#pragma once

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_core_exception.h"
#include "armvm_core_mem.h"

#include "armvm_core.h"

#include "local/core_reg.h"
#include "local/ldst.h"

/* **** */

#include "libarm/include/arm_ir.h"

/* **** */

#include <stdint.h>

/* **** */

static int __ldr_arm(armvm_core_p const core)
{ return(__ldr(core, 1)); }

static uint32_t __ldst_arm__ea(armvm_core_p const core)
{
	const unsigned bit_p = ARM_IR_LDST_BIT(P);

	assert(!(!bit_p && ARM_IR_LDST_BIT(W)));

	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	const uint32_t offset = ARM_IR_LDST_BIT(U) ? vR(SOP) : -vR(SOP);

	const uint32_t pre_offset = bit_p ? offset : 0;
	const uint32_t post_offset = bit_p ? 0 : offset;

	const uint32_t ea = setup_rR_vR(core, ARMVM_TRACE_R(EA), ~0, rn + pre_offset);

	return(ea + post_offset);
}

static void __ldst_arm__ea_wb(armvm_core_p const core, uint32_t wb_ea)
{
	if((!ARM_IR_LDST_BIT(P)) || ARM_IR_LDST_BIT(W))
		core_reg_wb_v(core, ARMVM_TRACE_R(N), wb_ea);
}

static uint32_t __ldst_arm_ea_fn(armvm_core_p const core, ldst_fn fn)
{
	const uint32_t wb_ea = __ldst_arm__ea(core);

	if(!CCX) return(0);

	int rval = fn(core);
	if(1 == rval)
		__ldst_arm__ea_wb(core, wb_ea);

	return(rval);
}
