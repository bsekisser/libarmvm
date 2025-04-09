#pragma once

/* **** */

#include "armvm_core.h"

#include "local/ldstm.h"

/* **** */

#include <stdint.h>

/* **** */

static int arm_ldm(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__ldm(core, r, ea));
}

static int arm_ldm_user(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);

	uint32_t v = 0;
	const int rval = armvm_core_mem_read(core, &v, ea, 4);
	if(0 <= rval)
		armvm_core_reg_user(core, r, &v);

	return(rval);
}

static int arm_ldm_pc(armvm_core_ref core, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__ldm_pc(core, ea));
}

static int arm_stm(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__stm(core, r, ea));
}

static int arm_stm_user(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);

	uint32_t v = armvm_core_reg_user(core, r, 0);
	return(armvm_core_mem_write(core, ea, 4, v));
}
