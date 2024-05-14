#pragma once

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_core_exception.h"
#include "armvm_core_mem.h"
#include "armvm_core.h"

/* **** */

#include "local/core_reg.h"

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"

/* **** */

/* **** */

#include "libbse/include/shift_roll.h"

/* **** */

#include <assert.h>

/* **** */

typedef int (*ldst_fn)(armvm_core_p const core);

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

	if(rR_IS_PC(D))
		armvm_core_pcx_v5(core, vR(D));
	else
		core_reg_wb(core, ARMVM_TRACE_R(D));

	return(1);
}

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

	return(1);
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

	return(1);
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

	return(1);
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

	return(1);
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
