#pragma once

/* **** */

#include "armvm_core.h"

#include "local/alubox_flags.h"

/* **** */

#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_dp.h"

/* **** */

#include "libbse/include/unused.h"

/* **** */

#include <stdint.h>

/* **** */

static uint32_t alubox(armvm_core_p const core, const unsigned operation,
	const unsigned s, const unsigned nzc)
{
	uint32_t sop = 0;
	switch(operation) {
		case ARM_BIC:
		case ARM_MVN:
			sop = ~vR(SOP);
			break;
		case ARM_RSB:
		case ARM_RSC:
			sop = core_reg_src_load(core, ARMVM_TRACE_R(N));
			break;
		default:
			sop = vR(SOP);
			break;
	}

	uint32_t rn = 0;
	switch(operation) {
		case ARM_MOV: break;
		case ARM_MVN: break;
		case ARM_RSB:
		case ARM_RSC:
			rn = vR(SOP);
			break;
		default:
			rn = core_reg_src_load(core, ARMVM_TRACE_R(N));
			break;
	}

	uint32_t rd = rn;

	switch(operation) {
		case ARM_ADC:
		case ARM_ADD:
		case ARM_CMN:
			rd += sop;
			break;
		case ARM_AND:
		case ARM_BIC:
		case ARM_TST:
			rd &= sop;
			break;
		case ARM_CMP:
		case ARM_NEG:
		case ARM_RSB:
		case ARM_RSC:
		case ARM_SBC:
		case ARM_SUB:
			rd -= sop;
if(0)		LOG("rn: 0x%08x, sop: 0x%08x, rd = 0x%08x, carry_in: %1u",
				rn, sop, rd, !!IF_CPSR(C));
			break;
		case ARM_EOR:
		case ARM_TEQ:
			rd ^= sop;
			break;
		case ARM_MOV:
		case ARM_MVN:
			rd = sop;
			break;
		case ARM_MUL:
			rd *= sop;
			break;
		case ARM_ORR:
			rd |= sop;
			break;
		default:
			arm_disasm(IP, IR);
			LOG_ACTION(exit(-1));
			break;
	}

	const unsigned carry_in = IF_CPSR(C);

	switch(operation) {
		case ARM_ADC:
			rd += !!carry_in;
			break;
		case ARM_RSC:
		case ARM_SBC:
			rd -= !!(!carry_in);
			break;
	}

	vR(D) = rd;
	if(!CCX) return(rd);

	switch(operation) {
		case ARM_CMN:
		case ARM_CMP:
		case ARM_TEQ:
		case ARM_TST:
			break;
		default:
			core_reg_wb(core, ARMVM_TRACE_R(D));
			break;;
	}

	if(s) {
		if(rR_IS_PC(D)) {
			if(pSPSR) armvm_core_psr_mode_switch_cpsr_spsr(core);
		} else {
			switch(operation) {
				case ARM_ADC:
				case ARM_ADD:
				case ARM_CMN:
				case ARM_NEG:
					_alubox_flags_x_add_sub(core, rd, rn, sop);
					break;
				case ARM_CMP:
				case ARM_RSB:
				case ARM_RSC:
				case ARM_SBC:
				case ARM_SUB:
					_alubox_flags_x_add_sub(core, rd, rn, ~sop);
					break;
				default:
					if(nzc)
						_alubox_flags_nzc(core, rd);
					else
						_alubox_flags_nz(core, rd);
					break;
			}
		}
	}

	return(rd);
}

UNUSED_FN
static uint32_t alubox_thumb(armvm_core_p const core, const unsigned operation,
	const unsigned s)
{
	unsigned shift_type = ~0U;
	switch(operation) {
		case ARM_ASR:
			shift_type = ARM_SOP_ASR;
			break;
		case ARM_LSL:
			shift_type = ARM_SOP_LSL;
			break;
		case ARM_LSR:
			shift_type = ARM_SOP_LSR;
			break;
		case ARM_ROR:
			shift_type = ARM_SOP_ROR;
			break;
		default:
			setup_vR(core, ARMVM_TRACE_R(SOP), vR(M));
			return(alubox(core, operation, s, 0));
	}

	const uint32_t valid_rs = vR(M) & 0xff;

	vR(SOP) = arm_shiftbox(~shift_type, vR(N), valid_rs, IF_CPSR(C));
	ARM_CPSR_BMAS(C, !!arm_shiftbox_c(~shift_type, vR(N), valid_rs));
	return(alubox(core, ARM_MOV, s, 0));
}
