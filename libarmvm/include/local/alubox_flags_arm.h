#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include "local/alubox_flags.h"
#include "local/alu_t.h"

/* **** */

#include <stdint.h>

/* **** */


/*static void alubox_flags_nz(armvm_core_p const core, alu_p const alu)
{
	if(!CCX) return;

	return(_alubox_flags_nz(core, alu->rd));
}*/

static void alubox_flags_nzc(armvm_core_p const core, alu_p const alu)
{
	if(!CCX) return;

	return(_alubox_flags_nzc(core, alu->rd));
}

static void alubox_fn_flags_nzc(armvm_core_p const core, alu_p const alu, alu_fn const fn)
{
	fn(core, alu);
	alubox_flags_nzc(core, alu);
}
