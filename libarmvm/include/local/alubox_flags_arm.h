#pragma once

/* **** */

#include "armvm_core.h"

/* **** */

#include "local/alu_t.h"
#include "local/alubox_flags.h"

/* **** */

#include <stdint.h>

/* **** */


/*static void alubox_flags_nz(armvm_core_ref core)
{
	if(!CCX) return;

	return(_alubox_flags_nz(core, vR(D)));
}*/

static void alubox_flags_nzc(armvm_core_ref core)
{
	if(!CCX) return;

	return(_alubox_flags_nzc(core, vR(D)));
}

static void alubox_fn_flags_nzc(armvm_core_ref core, alu_fn const fn)
{
	fn(core);
	alubox_flags_nzc(core);
}
