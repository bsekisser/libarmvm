#pragma once

/* **** */

#include "armvm_core.h"

#include "local/core_ldstm.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t _thumb_ldstmia_ea(armvm_core_p const core, uint32_t *const p2ea)
{
	const uint32_t ea = *p2ea;
	*p2ea += 4;

	return(ea);
	UNUSED(core);
}

/* **** */

static int thumb_ldmia(armvm_core_p const core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = _thumb_ldstmia_ea(core, p2ea);
	return(core_ldm(core, r, ea));
}

static int thumb_ldmia_pc(armvm_core_p const core, uint32_t *const p2ea)
{
	const int ldmia_rval = thumb_ldmia(core, ARMVM_GPR(PC), p2ea);

	const int thumb = PC & 1;
	PC &= ~(3 >> thumb);

	ARM_CPSR_BMAS(Thumb, thumb);

	return(ldmia_rval);
}

static int thumb_stmia(armvm_core_p const core, const unsigned r, uint32_t *const p2ea)
{
	const uint32_t ea = _thumb_ldstmia_ea(core, p2ea);
	return(core_stm(core, r, ea));
}
