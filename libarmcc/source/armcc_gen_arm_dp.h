#pragma once

/* **** */

extern "C" {
	#include "armcc_gen_arm_ir.h"
	#include "armcc_gen.h"
	#include "armcc_t.h"
}

/* **** */

extern "C" {
	#include "libarm/include/arm_dp.h"
}

/* **** */

extern "C" {
	#include <stdint.h>
}

/* **** */

static uint32_t gen_arm_dp__op_s_rd_rn_sop(armcc_p const p2cc, const unsigned group,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop)
{
	const uint32_t ir = gen_arm_ir__group_x(group,
		_gen_arm_ir__dp_opcode(opcode, s)
			| _gen_arm_ir__rd_rn(rd, rn)
			| sop);

	return(armcc_gen_ir(p2cc, ir);
}

static uint32_t gen_arm_dp__op_s_rd_rn_rm(armcc_p const p2cc,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	const uint32_t ir = gen_arm_ir__group_x(p2cc, 0,
		_gen_arm_ir__dp_opcode(opcode, s)
			| gen_arm_ir__rd_rn_rm(rd, rn, rm));

	return(armcc_gen_ir(p2cc, ir));
}
