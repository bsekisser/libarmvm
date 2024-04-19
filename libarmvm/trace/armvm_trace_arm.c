#include "armvm_core_trace.h"

/* **** */

#include "armvm_trace_glue.h"
#include "armvm_trace_arm.h"
#include "armvm_trace.h"

/* **** */

#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_sop.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

static void __arm_decode_fail(armvm_trace_p const atp)
{ LOG_ACTION(exit(-1)); UNUSED(atp); }

static void _armvm_trace_b_bl_blx(armvm_trace_p const atp, const uint32_t new_pc, const int link, const int blx)
{
	if(!atp) return;

	_armvm_trace(atp, "b%s%s(0x%08x)",
		link ? "l" : "",
		blx ? "x" : "", new_pc);
}

static void _armvm_trace_bx_blx_m(armvm_trace_p const atp, const int link)
{
	if(!atp) return;

	_armvm_trace_start(atp, "b%sx(%s)",
		link ? "l" : "", rR_NAME(M));

	const int thumb = 1 & vR(M);
	_armvm_trace_comment(atp, "%s(0x%08x)", thumb ? "T" : "A", vR(M));

	__trace_end(atp);
}

static void _dp_mov_s_s(armvm_trace_p const atp)
{
	if(!ARM_IR_DPI) {
		if(mlBFEXT(IR, 11, 4)) {
			_armvm_trace_comment(atp, "%s(0x%08x, %u) = 0x%08x",
				arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE],
					vR(M), vR(S), vR(D));
		}
		else if(rR(D) == rR(M))
		{
			_armvm_trace_comment(atp, "nop");
		}
		else
		{
			_armvm_trace_comment(atp, "0x%08x", vR(D));
		}
	} else {
		if(vR(S)) {
			_armvm_trace_comment(atp, "ROR(0x%08x, %u) = 0x%08x",
				vR(M), vR(S), vR(D));
		} else {
			_armvm_trace_comment(atp, "0x%08x", vR(D)); // TODO: rrx?
		}
	}
}

/* **** */

void armvm_trace_b_bl(armvm_trace_p const atp)
{
	const uint32_t new_pc = ARM_PC_NEXT + ARM_IR_B_OFFSET;
	return(_armvm_trace_b_bl_blx(atp, new_pc, ARM_IR_B_LINK, 0));
}

void armvm_trace_blx(armvm_trace_p const atp)
{
	const uint32_t new_pc = ARM_PC_NEXT + ARM_IR_BLX_OFFSET;
	return(_armvm_trace_b_bl_blx(atp, new_pc, ARM_IR_B_LINK, 1));
}

void armvm_trace_blx_m(armvm_trace_p const atp)
{ return(_armvm_trace_bx_blx_m(atp, 1)); }

void armvm_trace_bx_m(armvm_trace_p const atp)
{ return(_armvm_trace_bx_blx_m(atp, 0)); }

void armvm_trace_dp(armvm_trace_p const atp)
{
	if(!__trace_start(atp))
		return;

	_armvm_trace_(atp, "%s%s(",
		arm_dp_inst_string[ARM_IR_DP_OPCODE], ARM_IR_DP_S ? "s" : "");

	if(ARM_IR_DP_WB)
		_armvm_trace_(atp, "%s", rR_NAME(D));

	switch(ARM_IR_DP_OPCODE) {
		case ARM_MOV:
		case ARM_MVN:
			break;
		default:
			_armvm_trace_(atp, "%s%s", ARM_IR_DP_WB ? ", " : "", rR_NAME(N));
			break;
	}

	if(ARM_IR_DPI)
	{
		if(vR(S)) {
			_armvm_trace_(atp, ", ROR(%u, %u))", vR(M), vR(S));
		} else
			_armvm_trace_(atp, ", %u)", vR(M));
	}
	else
	{
		const char* sos = arm_sop_lcase_string[ARM_IR_DP_SHIFT_TYPE];

		if(ARM_IR_DP_I)
			_armvm_trace_(atp, ", %s(%s, %s))", sos, rR_NAME(M), rR_NAME(S));
		else {
			if(!mlBFEXT(IR, 11, 4))
				_armvm_trace_(atp, ", %s)", rR_NAME(M));
			else {
				switch(ARM_IR_DP_SHIFT_TYPE) {
					case ARM_SOP_ROR:
						if(!vR(S))
							_armvm_trace_(atp, ", RRX(%s))", sos, rR_NAME(M));
						break;
					default:
							_armvm_trace_(atp, ", %s(%s, %u))", sos, rR_NAME(M), vR(S));
						break;
				}
			}
		}
	}

	const char* dp_op_string = arm_dp_op_string[ARM_IR_DP_OPCODE];

	switch(ARM_IR_DP_OPCODE) {
		default:
			_armvm_trace_comment(atp, "0x%08x %s0x%08x --> 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_BIC:
			_armvm_trace_comment(atp, "0x%08x & ~0x%08x(0x%08x) --> 0x%08x",
				vR(N), vR(SOP), ~vR(SOP), vR(D));
			break;
		case ARM_CMP:
		case ARM_CMN:
		case ARM_TEQ:
		case ARM_TST:
			_armvm_trace_comment(atp, "0x%08x %s0x%08x ??? 0x%08x",
				vR(N), dp_op_string, vR(SOP), vR(D));
			break;
		case ARM_MOV:
			_dp_mov_s_s(atp);
			break;
		case ARM_MVN:
			_armvm_trace_comment(atp, "0x%08x", vR(D));
			break;
		case ARM_RSB:
		case ARM_RSC:
			_armvm_trace_comment(atp, "0x%08x - 0x%08x --> 0x%08x",
				vR(SOP), vR(N), vR(D));
			break;
	}

	__trace_end(atp);
}

void armvm_trace_ldst(armvm_trace_p const atp)
{
	if(!__trace_start(atp))
		return;

//	int is_ld = 0;

	/* ldr|str{cond}{b}{t} <rd>, addressing mode */
	/* ldr|str{cond}{h|sh|sb|b} <rd>, addressing mode */

	switch(ARM_IR_GROUP & ~1) {
	case 0:
//		is_ld = ARM_IR_LDST_SH_FLAG_L;
		_armvm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_L ? "ldr" : "str");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "s" : "");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "h" : "");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "b" : "");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_SH_FLAG_S ? "d" : "");
	break;
	case 2:
//		is_ld = ARM_IR_LDST_BIT(L);
		_armvm_trace_(atp, ARM_IR_LDST_BIT(L) ? "ldr" :  "str");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_BIT(B) ? "B" : "");
		_armvm_trace_(atp, "%s", ARM_IR_LDST_FLAG_T ? "T" : "");
	break;
	default:
		LOG_ACTION(return(__arm_decode_fail(atp)));
	};

	const unsigned bit_p = ARM_IR_LDST_BIT(P);
	const unsigned bit_w = ARM_IR_LDST_BIT(W);

//	const unsigned wb = !bit_p || (bit_p && ARM_IR_LDST_BIT(W));
	const unsigned wb = (!bit_p && !bit_w) || (bit_p && bit_w);

	_armvm_trace_(atp, "(%s, %s%s", rR_NAME(D), rR_NAME(N), wb ? "!" : "");

	switch(ARM_IR_GROUP) {
	case 0:
		if(ARM_IR_LDST_SH_BIT(I)) {
			_armvm_trace_(atp, ", %s0x%04x)",
				ARM_IR_LDST_BIT(U) ? "+" : "-",
				ARM_IR_LDST_SH_OFFSET);
		} else {
			_armvm_trace_(atp, ", %s%s)",
				ARM_IR_LDST_BIT(U) ? "" : "-", rR_NAME(M));
		}
	break;
	case 2:
		_armvm_trace_(atp, ", %c0x%04x)",
			ARM_IR_LDST_BIT(U) ? '+' : '-',
			ARM_IR_LDST_IMMEDIATE_OFFSET);
	break;
	default:
		LOG("ARM_IR_GROUP: %01u", ARM_IR_GROUP);
		LOG_ACTION(return(__arm_decode_fail(atp)));
	}

	if(pARMVM_CORE)
		_armvm_trace_comment(atp, "[0x%08x]: 0x%08x", vR(EA), vR(D));

	__trace_end(atp);
}

void armvm_trace_mcr_mrc(armvm_trace_p const atp)
{
	if(!atp) return;

	_armvm_trace(atp, "m%s(p(%u), %u, %s, %s, %s, %u)",
		ARM_IR_MCRC_L ? "rc" : "cr", ARM_IR_MCRC_CPx, ARM_IR_MCRC_OP1,
		rR_NAME(D),
		arm_creg_name_string[ARM_IR_MCRC_CRn], arm_creg_name_string[ARM_IR_MCRC_CRm],
		ARM_IR_MCRC_OP2);
}

void armvm_trace_mla(armvm_trace_p const atp)
{
	if(!__trace_start(atp))
		return;

	_armvm_trace_(atp, "mla%s(", ARM_IR_DP_S ? "s" : "");
	_armvm_trace_(atp, "%s", rR_NAME(D));
	_armvm_trace_(atp, ", %s", rR_NAME(M));
	_armvm_trace_(atp, ", %s", rR_NAME(S));
	_armvm_trace_(atp, ", %s", rR_NAME(N));

	if(pARMVM_CORE)
		_armvm_trace_comment(atp, "(0x%08x * 0x%08x) + 0x%08x = 0x%08x",
			vR(M), vR(S), vR(N), vR(D));

	__trace_end(atp);
}

void armvm_trace_mrs(armvm_trace_p const atp)
{
	if(!atp) return;

	_armvm_trace_start(atp, "mrs(%s, %s)", rR_NAME(D), ARM_IR_MRSR_R ? "spsr" : "cpsr");
	_armvm_trace_comment(atp, "0x%08x", vR(D));
	__trace_end(atp);
}

void armvm_trace_msr(armvm_trace_p const atp)
{
	if(!__trace_start(atp))
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
		_armvm_trace_(atp, "msr(%cPSR_%s, 0x%08x)", cs, cpsrs, vR(SOP));
	else
		_armvm_trace_(atp, "msr(%cPSR_%s, %s)", cs, cpsrs, rR_NAME(M));

	_armvm_trace_comment(atp, "(0x%08x & 0x%08x): 0x%08x --> 0x%08x",
		vR(SOP), mask, operand_masked, vR(D));

	__trace_end(atp);
}

void armvm_trace_umull(armvm_trace_p const atp)
{
	if(!__trace_start(atp))
		return;

	_armvm_trace_(atp, "umull%s(", ARM_IR_DP_S ? "s" : "");
	_armvm_trace_(atp, "%s", rR_NAME(DLo));
	_armvm_trace_(atp, ":%s", rR_NAME(DHi));
	_armvm_trace_(atp, ", %s", rR_NAME(M));
	_armvm_trace_(atp, ", %s", rR_NAME(S));

	if(pARMVM_CORE) {
		_armvm_trace_comment(atp, "0x%08x * 0x%08x = 0x%08x%08x",
			vR(M), vR(S), vR(DHi), vR(DLo));
	}

	__trace_end(atp);
}
