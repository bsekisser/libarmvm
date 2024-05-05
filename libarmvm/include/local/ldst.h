#pragma once

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_core_exception.h"
#include "armvm_core_mem.h"

/* **** */

#include "local/core_reg.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/shift_roll.h"

/* **** */

#include <assert.h>

/* **** */

static int __ldr(armvm_core_p const core, const int arm)
{
	const unsigned ea_xx = vR(EA) & 3;

	if(ea_xx && CP15_REG1_BIT(A))
		return(armvm_core_exception_data_abort(core));

	const int read_rval = armvm_core_mem_read(core, &vR(D), vR(EA), 4);
	if(0 > read_rval)
		return(read_rval);

	if(arm) {
		if(ea_xx && CP15_REG1_BIT(U))
			vR(D) = _ror(vR(D), (ea_xx << 3));
	}

	core_reg_wb(core, ARMVM_TRACE_R(D));

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(Thumb, vR(D) & 1);

	return(rR_IS_PC(D));
}

UNUSED_FN
static int __ldr_arm(armvm_core_p const core)
{ return(__ldr(core, 1)); }

UNUSED_FN
static int __ldr_thumb(armvm_core_p const core)
{ return(__ldr(core, 0)); }

static int __ldrb(armvm_core_p const core)
{
	const int read_rval = armvm_core_mem_read(core, &vR(D), vR(EA), 1);
	if(0 > read_rval)
		return(read_rval);

	core_reg_wb(core, ARMVM_TRACE_R(D));

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(Thumb, vR(D) & 1);

	return(rR_IS_PC(D));
}

static int __ldrh(armvm_core_p const core)
{
	if((vR(EA) & 1) && CP15_REG1_BIT(A))
		return(armvm_core_exception_data_abort(core));

	const int read_rval = armvm_core_mem_read(core, &vR(D), vR(EA), 2);
	if(0 > read_rval)
		return(read_rval);

	core_reg_wb(core, ARMVM_TRACE_R(D));

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(Thumb, vR(D) & 1);

	return(rR_IS_PC(D));
}

UNUSED_FN
static int __ldrsb(armvm_core_p const core)
{
	const int read_rval = armvm_core_mem_read(core, &vR(D), vR(EA), 1);
	if(0 > read_rval)
		return(read_rval);

	vR(D) = (int32_t)(int8_t)vR(D);

	core_reg_wb(core, ARMVM_TRACE_R(D));

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(Thumb, vR(D) & 1);

	return(rR_IS_PC(D));
}

UNUSED_FN
static int __ldrsh(armvm_core_p const core)
{
	if((vR(EA) & 1) && CP15_REG1_BIT(A))
		return(armvm_core_exception_data_abort(core));

	const int read_rval = armvm_core_mem_read(core, &vR(D), vR(EA), 2);
	if(0 > read_rval)
		return(read_rval);

	vR(D) = (int32_t)(int16_t)vR(D);

	core_reg_wb(core, ARMVM_TRACE_R(D));

	if(rR_IS_PC(D) && (arm_v5t <= CONFIG->version))
		ARM_CPSR_BMAS(Thumb, vR(D) & 1);

	return(rR_IS_PC(D));
}

UNUSED_FN
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

UNUSED_FN
static void __ldst_arm__ea_wb(armvm_core_p const core, uint32_t wb_ea)
{
	if(!CCX) return;

	if(!ARM_IR_LDST_BIT(P) || ARM_IR_LDST_BIT(W))
		irGPR(N) = wb_ea;
}

static int __str(armvm_core_p const core)
{
	core_reg_src_load(core, ARMVM_TRACE_R(D));

	if((vR(EA) & 3) && CP15_REG1_BIT(A))
		return(armvm_core_exception_data_abort(core));

	return(armvm_core_mem_write(core, vR(EA), 4, vR(D)));
}

static int __strb(armvm_core_p const core)
{
	core_reg_src_load(core, ARMVM_TRACE_R(D));

	return(armvm_core_mem_write(core, vR(EA), 1, vR(D)));
}

static int __strh(armvm_core_p const core)
{
	core_reg_src_load(core, ARMVM_TRACE_R(D));

	if((vR(EA) & 1) && CP15_REG1_BIT(A))
		return(armvm_core_exception_data_abort(core));

	return(armvm_core_mem_write(core, vR(EA), 2, vR(D)));
}
