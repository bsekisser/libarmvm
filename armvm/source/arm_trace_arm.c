#include "armvm_core_trace.h"

/* **** */

#include "arm_trace_glue.h"
#include "arm_trace_arm.h"
#include "arm_trace.h"

/* **** */

#include "arm_dp.h"
#include "arm_ir.h"
#include "arm_sop.h"
#include "arm_strings.h"

/* **** */

#include "bitfield.h"
#include "log.h"

/* **** */

static void __arm_decode_fail(arm_trace_p atp)
{ LOG_ACTION(exit(-1)); }

static void _arm_trace_b_bl_blx(uint32_t new_pc, int link, int blx, arm_trace_p atp)
{
	_arm_trace(pARM_TRACE, "b%s%s(0x%08x)",
		link ? "l" : "",
		blx ? "x" : "", new_pc);
}

static void _dp_mov_s_s(arm_trace_p atp)
{
	if(!ARM_IR_DPI) {
		if(mlBFEXT(IR, 11, 4)) {
			_arm_trace_comment(atp, "%s(0x%08x, %03u) = 0x%08x",
				arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE],
					vR(M), vR(S), vR(D));
		}
		else if(rR(D) == rR(M))
		{
			_arm_trace_comment(atp, "nop");
		}
		else
		{
			_arm_trace_comment(atp, "0x%08x", vR(D));
		}
	} else {
		if(vR(S)) {
			_arm_trace_comment(atp, "ROR(0x%08x, %03u) = 0x%08x",
				vR(M), vR(S), vR(D));
		} else {
			_arm_trace_comment(atp, "0x%08x", vR(D)); // TODO: rrx?
		}
	}
}

/* **** */

void arm_trace_b_bl(arm_trace_p atp)
{
	const uint32_t new_pc = ARM_PC_NEXT + ARM_IR_B_OFFSET;
	return(_arm_trace_b_bl_blx(new_pc, ARM_IR_B_LINK, 0, atp));
}

void arm_trace_dp(arm_trace_p atp)
{
	if(!__trace_start(atp))
		return;

	_arm_trace_(atp, "%s%s(",
		arm_dp_inst_string[ARM_IR_DP_OPCODE], ARM_IR_DP_S ? "s" : "");

	if(ARM_IR_DP_WB)
		_arm_trace_(atp, "%s", rR_NAME(D));

	switch(ARM_IR_DP_OPCODE) {
		case ARM_MOV:
		case ARM_MVN:
			break;
		default:
			_arm_trace_(atp, "%s%s", ARM_IR_DP_WB ? ", " : "", rR_NAME(N));
			break;
	}

	if(ARM_IR_DPI)
	{
		if(vR(S)) {
			_arm_trace_(atp, ", ROR(%u, %u))", vR(M), vR(S));
		} else
			_arm_trace_(atp, ", %u)", vR(M));
	}
	else
	{
		const char* sos = arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE];

		if(ARM_IR_DP_I)
			_arm_trace_(atp, ", %s(%s, %s))", sos, rR_NAME(M), rR_NAME(S));
		else {
			if(!mlBFEXT(IR, 11, 4))
				_arm_trace_(atp, ", %s)", rR_NAME(M));
			else {
				switch(ARM_IR_DP_SHIFT_TYPE) {
					case ARM_SOP_ROR:
						if(!vR(S))
							_arm_trace_(atp, ", RRX(%s))", sos, rR_NAME(M));
						break;
					default:
							_arm_trace_(atp, ", %s(%s, %u))", sos, rR_NAME(M), vR(S));
						break;
				}
			}
		}
	}

	const char* dp_op_string = arm_dp_op_string[ARM_IR_DP_OPCODE];

	switch(ARM_IR_DP_OPCODE) {
		default:
			_arm_trace_comment(atp, "0x%08x %s0x%08x --> 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_BIC:
			_arm_trace_comment(atp, "0x%08x & ~0x%08x(0x%08x) --> 0x%08x",
				vR(N), vR(SOP), ~vR(SOP), vR(D));
			break;
		case ARM_CMP:
		case ARM_CMN:
		case ARM_TEQ:
		case ARM_TST:
			_arm_trace_comment(atp, "0x%08x %s0x%08x ??? 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_MOV:
			_dp_mov_s_s(atp);
			break;
		case ARM_MVN:
			_arm_trace_comment(atp, "0x%08x", vR(D));
			break;
		case ARM_RSB:
		case ARM_RSC:
			_arm_trace_comment(atp, "0x%08x - 0x%08x --> 0x%08x",
				vR(SOP), vR(N), vR(D));
			break;
	}

	__trace_end(atp);
}

void arm_trace_ldst(arm_trace_p atp)
{
	if(!__trace_start(atp))
		return;

	int is_ld = 0;

	/* ldr|str{cond}{b}{t} <rd>, addressing mode */
	/* ldr|str{cond}{h|sh|sb|b} <rd>, addressing mode */

	switch(ARM_IR_GROUP & ~1) {
	case 0:
		is_ld = ARM_IR_LDST_SH_FLAG_L;
		_arm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_L ? "ldr" : "str");
		_arm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "s" : "");
		_arm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "h" : "");
		_arm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "b" : "");
		_arm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "d" : "");
	break;
	case 2:
		is_ld = ARM_IR_LDST_BIT(L);
		_arm_trace_(atp, ARM_IR_LDST_BIT(L) ? "ldr" :  "str");
		_arm_trace_(atp, "%s", ARM_IR_LDST_BIT(B) ? "B" : "");
		_arm_trace_(atp, "%s", ARM_IR_LDST_FLAG_T ? "T" : "");
	break;
	default:
		LOG_ACTION(return(__arm_decode_fail(atp)));
	};

	const unsigned bit_p = ARM_IR_LDST_BIT(P);
	const unsigned bit_w = ARM_IR_LDST_BIT(W);

//	const unsigned wb = !bit_p || (bit_p && ARM_IR_LDST_BIT(W));
	const unsigned wb = (!bit_p && !bit_w) || (bit_p && bit_w);

	_arm_trace_(atp, "(%s, %s%s", rR_NAME(D), rR_NAME(N), wb ? "!" : "");

	switch(ARM_IR_GROUP) {
	case 0:
		if(ARM_IR_LDST_SH_BIT(I)) {
			_arm_trace_(atp, ", %s0x%04x)",
				ARM_IR_LDST_BIT(U) ? "+" : "-",
				ARM_IR_LDST_SH_OFFSET);
		} else {
			_arm_trace_(atp, ", %s%s)",
				ARM_IR_LDST_BIT(U) ? "" : "-", rR_NAME(M));
		}
	break;
	case 2:
		_arm_trace_(atp, ", %c0x%04x)",
			ARM_IR_LDST_BIT(U) ? '+' : '-',
			ARM_IR_LDST_IMMEDIATE_OFFSET);
	break;
	default:
		LOG("ARM_IR_GROUP: %01u", ARM_IR_GROUP);
		LOG_ACTION(return(__arm_decode_fail(atp)));
	}

	if(atp->arm_vm)
		_arm_trace_comment(atp, "[0x%08x]: 0x%08x", vR(EA), vR(D));

	__trace_end(atp);
}

void arm_trace_mla(arm_trace_p atp)
{
	if(!__trace_start(atp))
		return;

	_arm_trace_(atp, "mla%s(", ARM_IR_DP_S ? "s" : "");
	_arm_trace_(atp, "%s", rR_NAME(D));
	_arm_trace_(atp, ", %s", rR_NAME(M));
	_arm_trace_(atp, ", %s", rR_NAME(S));
	_arm_trace_(atp, ", %s", rR_NAME(N));

	if(atp->arm_vm)
		_arm_trace_comment(atp, "(0x%08x * 0x%08x) + 0x%08x = 0x%08x",
			vR(M), vR(S), vR(N), vR(D));

	__trace_end(atp);
}

void arm_trace_msr(arm_trace_p atp)
{
	_arm_trace_start(atp, "mrs(%s, %s)", rR_NAME(D), ARM_IR_MSR_R ? "spsr" : "cpsr");
	_arm_trace_comment(atp, "0x%08x", vR(D));
	__trace_end(atp);
}

void arm_trace_umull(arm_trace_p atp)
{
	if(!__trace_start(atp))
		return;

	_arm_trace_(atp, "umull%s(", ARM_IR_DP_S ? "s" : "");
	_arm_trace_(atp, "%s", rR_NAME(DLo));
	_arm_trace_(atp, ":%s", rR_NAME(DHi));
	_arm_trace_(atp, ", %s", rR_NAME(M));
	_arm_trace_(atp, ", %s", rR_NAME(S));

	if(atp->arm_vm) {
		_arm_trace_comment(atp, "0x%08x * 0x%08x = 0x%08x%08x",
			vR(M), vR(S), vR(DHi), vR(DLo));
	}

	__trace_end(atp);
}
