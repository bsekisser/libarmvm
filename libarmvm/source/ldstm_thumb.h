#pragma once

/* **** */

#include "armvm_core.h"

#include "ldstm.h"

/* **** */

#include <stdint.h>

/* **** */

static int thumb_ldmia(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__ldm(core, r, ea));
}

static int thumb_ldmia_pc(armvm_core_ref core, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__ldm_pc(core, ea));
}

static int thumb_stmia(armvm_core_ref core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = __ldstm_ea(core, p2ea);
	return(__stm(core, r, ea));
}
