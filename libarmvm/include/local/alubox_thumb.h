#pragma once

/* **** */

#include "local/alubox_flags.h"
#include "local/core_reg_t.h"

/* **** */

#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include <stdint.h>

/* **** */

void alubox_thumb(armvm_core_p const core, const unsigned operation, const unsigned s,
	reg_p const p2rd, reg_p const p2rn, const uint32_t sop)
{
	uint32_t rn = 0;

	switch(operation) {
		case ARM_MOV: break;
		default:
			rn = core_reg_t_src(core, p2rn);
		break;
	}

	uint32_t rd = rn;

	switch(operation) {
		case ARM_ADD:
		case ARM_MOV:
			rd += sop;
		break;
		case ARM_CMP:
		case ARM_SUB:
			rd -= sop;
		break;
		default:
			arm_disasm(IP, IR);
			LOG("%s", arm_dp_inst_string[operation]);
			LOG_ACTION(abort());
		break;
	}

	p2rd->v = rd;

	if(8 != (operation & ~3))
		core_reg_t_dst(core, p2rd);

	if(s) switch(operation) {
		case ARM_ADD:
			_alubox_flags_x_add_sub(core, rd, rn, sop);
		break;
		case ARM_CMP:
		case ARM_SUB:
			_alubox_flags_x_add_sub(core, rd, ~rn, sop);
		break;
		default:
			_alubox_flags_nz(core, rd);
		break;
	}
}
