#pragma once

/* **** */

#include "armvm_core_glue.h"

#include "armvm_core_trace.h"
#include "armvm_core.h"

/* **** */

#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include "local/core_reg.h"

/* **** */

#include <stdint.h>

/* **** */

static void _dp_mov_s_s(armvm_core_p const core)
{
	if(ARM_IR_DPI) {
		if(vR(S)) {
			_armvm_trace_comment(core, "ROR(0x%08x, %u) = 0x%08x",
				vR(M), vR(S), vR(D));
		} else {
			_armvm_trace_comment(core, "0x%08x", vR(D));
		}
	} else {
		if(mlBFEXT(IR, 11, 4)) {
			_armvm_trace_comment(core, "%s(0x%08x, %u) = 0x%08x",
				arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE],
					vR(M), vR(S), vR(D));
		}
		else if(rR(D) == rR(M))
		{
			_armvm_trace_comment(core, "nop");
		}
		else
		{
			_armvm_trace_comment(core, "0x%08x", vR(D));
		}
	}
}

static void armvm_trace_dp(armvm_core_p const core)
{
	if(!__trace_start(core))
		return;

	_armvm_trace_(core, "%s%s(",
		arm_dp_inst_string[ARM_IR_DP_OPCODE], ARM_IR_DP_S ? "s" : "");

	if(ARM_IR_DP_WB)
		_armvm_trace_(core, "%s", irR_NAME(D));

	switch(ARM_IR_DP_OPCODE) {
		case ARM_MOV:
		case ARM_MVN:
			break;
		default:
			_armvm_trace_(core, "%s%s", ARM_IR_DP_WB ? ", " : "", irR_NAME(N));
			break;
	}

	if(ARM_IR_DPI)
	{
		if(vR(S)) {
			_armvm_trace_(core, ", ROR(%u, %u))", vR(M), vR(S));
		} else
			_armvm_trace_(core, ", %u)", vR(M));
	}
	else
	{
		const char* sos = arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE];

		if(ARM_IR_DP_I)
			_armvm_trace_(core, ", %s(%s, %s))", sos, irR_NAME(M), irR_NAME(S));
		else {
			if(!mlBFEXT(IR, 11, 4))
				_armvm_trace_(core, ", %s)", irR_NAME(M));
			else {
				switch(ARM_IR_DP_SHIFT_TYPE) {
					case ARM_SOP_ROR:
						if(!vR(S))
							_armvm_trace_(core, ", RRX(%s))", sos, irR_NAME(M));
						break;
					default:
							_armvm_trace_(core, ", %s(%s, %u))", sos, irR_NAME(M), vR(S));
						break;
				}
			}
		}
	}

	const char* dp_op_string = arm_dp_op_string[ARM_IR_DP_OPCODE];

	switch(ARM_IR_DP_OPCODE) {
		default:
			_armvm_trace_comment(core, "0x%08x %s0x%08x --> 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_BIC:
			_armvm_trace_comment(core, "0x%08x & ~0x%08x(0x%08x) --> 0x%08x",
				vR(N), vR(SOP), ~vR(SOP), vR(D));
			break;
		case ARM_CMP:
		case ARM_CMN:
		case ARM_TEQ:
		case ARM_TST:
			_armvm_trace_comment(core, "0x%08x %s0x%08x ??? 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_MOV:
			_dp_mov_s_s(core);
			break;
		case ARM_MVN:
			_armvm_trace_comment(core, "0x%08x", vR(D));
			break;
		case ARM_RSB:
		case ARM_RSC:
			_armvm_trace_comment(core, "0x%08x - 0x%08x --> 0x%08x",
				vR(SOP), vR(N), vR(D));
			break;
	}

	__trace_end(core);
}

void armvm_trace_ldst(armvm_core_p const core)
{
	if(!__trace_start(core))
		return;

//	int is_ld = 0;

	/* ldr|str{cond}{b}{t} <rd>, addressing mode */
	/* ldr|str{cond}{h|sh|sb|b} <rd>, addressing mode */

	switch(ARM_IR_GROUP & ~1) {
	case 0:
//		is_ld = ARM_IR_LDST_SH_FLAG_L;
		_armvm_trace_(core, "%s", ARM_IR_LDST_SH_FLAG_L ? "ldr" : "str");
		_armvm_trace_(core, "%s", ARM_IR_LDST_SH_FLAG_S ? "s" : "");
		_armvm_trace_(core, "%s", ARM_IR_LDST_SH_FLAG_H ? "h" : "");
		_armvm_trace_(core, "%s", ARM_IR_LDST_SH_FLAG_B ? "b" : "");
		_armvm_trace_(core, "%s", ARM_IR_LDST_SH_FLAG_D ? "d" : "");
	break;
	case 2:
//		is_ld = ARM_IR_LDST_BIT(L);
		_armvm_trace_(core, ARM_IR_LDST_BIT(L) ? "ldr" :  "str");
		_armvm_trace_(core, "%s", ARM_IR_LDST_BIT(B) ? "b" : "");
		_armvm_trace_(core, "%s", ARM_IR_LDST_FLAG_T ? "t" : "");
	break;
	default:
		LOG_ACTION(arm_disasm(IP, IR); exit(-1));
	};

	const unsigned bit_p = ARM_IR_LDST_BIT(P);
	const unsigned bit_w = ARM_IR_LDST_BIT(W);

//	const unsigned wb = !bit_p || (bit_p && ARM_IR_LDST_BIT(W));
	const unsigned wb = (!bit_p && !bit_w) || (bit_p && bit_w);

	_armvm_trace_(core, "(%s, %s%s", irR_NAME(D), irR_NAME(N), wb ? "!" : "");

	switch(ARM_IR_GROUP) {
	case 0:
		if(ARM_IR_LDST_SH_BIT(I)) {
			_armvm_trace_(core, ", %s0x%04x)",
				ARM_IR_LDST_BIT(U) ? "+" : "-",
				ARM_IR_LDST_SH_OFFSET);
		} else {
			_armvm_trace_(core, ", %s%s)",
				ARM_IR_LDST_BIT(U) ? "" : "-", irR_NAME(M));
		}
	break;
	case 2:
		_armvm_trace_(core, ", %c0x%04x)",
			ARM_IR_LDST_BIT(U) ? '+' : '-',
			ARM_IR_LDST_IMMEDIATE_OFFSET);
	break;
	case 3:
		if(!mlBFEXT(IR, 11, 4))
			_armvm_trace_(core, ", %s)", irR_NAME(M));
		else {
			const char* sos = arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE];
			const char* rrx = arm_sop_lcase_string[ARM_SOP_RRX];

			switch(ARM_IR_DP_SHIFT_TYPE) {
				case ARM_SOP_ROR:
					if(!vR(S))
						_armvm_trace_(core, ", %s(%s))", rrx, irR_NAME(M));
					break;
				default:
						_armvm_trace_(core, ", %s(%s, %u))", sos, irR_NAME(M), vR(S));
					break;
			}
		}
	break;
	default:
		LOG("ARM_IR_GROUP: %01u", ARM_IR_GROUP);
		LOG_ACTION(arm_disasm(IP, IR); exit(-1));
	}

	if(CCX)
		_armvm_trace_comment(core, "[0x%08x]: 0x%08x", vR(EA), vR(D));

	__trace_end(core);
}

static void armvm_trace_msr(armvm_core_p const core)
{
	if(!__trace_start(core))
		return;

	const unsigned field_mask = ~rR(S);

	uint8_t cpsrs[5];
	cpsrs[0] = BTST(field_mask, 3) ? 'F' : 'f';
	cpsrs[1] = BTST(field_mask, 2) ? 'S' : 's';
	cpsrs[2] = BTST(field_mask, 1) ? 'X' : 'x';
	cpsrs[3] = BTST(field_mask, 0) ? 'C' : 'c';
	cpsrs[4] = 0;

	const uint8_t cs = ARM_IR_MRSR_R ? 'S' : 'C';

	const uint32_t mask = vR(S);
	const uint32_t operand_masked = vR(SOP) & mask;

	if(ARM_IR_DPI)
		_armvm_trace_(core, "msr(%cPSR_%s, 0x%08x)", cs, cpsrs, vR(SOP));
	else
		_armvm_trace_(core, "msr(%cPSR_%s, %s)", cs, cpsrs, irR_NAME(M));

	_armvm_trace_comment(core, "(0x%08x & 0x%08x): 0x%08x --> 0x%08x",
		vR(SOP), mask, operand_masked, vR(D));

	__trace_end(core);
}
