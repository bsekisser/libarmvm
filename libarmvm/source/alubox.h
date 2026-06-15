#pragma once

/* **** */

#include "armvm_core.h"

#include "alubox_flags.h"
#include "reg.h"

/* **** */

#include "libbse/include/shift_roll_32.h"

#include "libarm/include/disasm.h"
#include "libarm/include/dp.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t alubox(armvm_core_ref core, arm_dp_opcode_eref operation,
	const unsigned s_in)
{
	const unsigned is_cmp = (010 == (~3 & operation));
	const unsigned is_mov = (015 == (~2 & operation));
	const unsigned mode_switch = s_in && CCX && rR_IS_PC(D);
	const unsigned s = s_in && CCX && rR_IS_NOT_PC(D);
	const unsigned wb = !is_cmp && CCX;

	const uint32_t rn = is_mov ? 0 : reg_src_load(core, rRN);
	const uint32_t sop = vR(SOP);

	uint32_t rd = 0;

	switch(operation) {
		case ARM_ADC:
			rd = flags_adc(core, s, rn, sop);
			break;
		case ARM_ADD: case ARM_CMN:
			rd = flags_add(core, s, rn, sop);
			break;
		case ARM_AND: case ARM_TST:
			rd = flags_nzc(core, s, rn & sop);
			break;
		case ARM_BIC:
			rd = flags_nzc(core, s, rn & ~sop);
			break;
		case ARM_CMP: case ARM_SUB:
			rd = flags_sub(core, s, rn, sop);
			break;
		case ARM_EOR: case ARM_TEQ:
			rd = flags_nzc(core,s , rn ^ sop);
			break;
		case ARM_MOV:
			rd = flags_nzc(core, s, sop);
			break;
		case ARM_MVN:
			rd = flags_nzc(core, s, ~sop);
			break;
		case ARM_ORR:
			rd = flags_nzc(core, s, rn | sop);
			break;
		case ARM_RSB:
			rd = flags_sub(core, s, sop, rn);
			break;
		case ARM_RSC:
			rd = flags_sbc(core, s, sop, rn);
			break;
		case ARM_SBC:
			rd = flags_sbc(core, s, rn, sop);
			break;
		default: switch((thumb_dp_opcode_eref)operation) {
			case ARM_ASR:
				rd = flags_nzc(core, s, asr32(rn, sop));
				break;
			case ARM_LSL:
				rd = flags_nzc(core, s, lsl32(rn, sop));
				break;
			case ARM_LSR:
				rd = flags_nzc(core, s, lsr32(rn, sop));
				break;
			case ARM_MUL:
				rd = flags_nz(core, s, rn * sop);
				break;
			case ARM_NEG:
				rd = flags_sub(core,s , 0, sop);
				break;
			case ARM_ROR:
				rd = flags_nzc(core, s, ror32(rn, sop));
				break;
			// should not get here
			default:
				LOG_ACTION(exit(-1));
		}
	}

	vR(D) = rd;

	if(wb)
		reg_wb(core, rRD);

	if(mode_switch && pSPSR)
		armvm_core_psr_mode_switch_cpsr_spsr(core);

	return(rd);
}
