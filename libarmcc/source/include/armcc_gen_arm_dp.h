#pragma once

/* **** */

#include "armcc_gen_arm.h"
#include "armcc_gen_arm_ir.h"
#include "armcc_t.h"

/* **** */

#include "libarm/include/arm_dp.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t gen_arm_dp__op_s_rd_rn(armcc_p const p2cc, const unsigned group,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop)
{
	return(gen_arm__group_ir(p2cc, group,
		gen_arm_ir__dp_opcode_s(opcode, s)
			| gen_arm_ir__rd_rn(rd, rn)
			| sop)
		);
}

static uint32_t gen_arm_dp__op_s_rd_rn_rm(armcc_p const p2cc,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	return(gen_arm__group_ir(p2cc, 0,
		gen_arm_ir__dp_opcode_s(opcode, s)
			| gen_arm_ir__rd_rn_rm(rd, rn, rm))
		);
}
