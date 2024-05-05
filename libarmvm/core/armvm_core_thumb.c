#define pARMVM_CORE core
#include "armvm_core_glue.h"

/* **** */

#include "armvm_core_cc.h"
#include "armvm_core_exception.h"
#include "armvm_core_mem.h"
#include "armvm_core_thumb.h"
#include "armvm_core_trace.h"

#include "armvm_coprocessor_cp15.h"

/* **** */

#include "armvm.h"

#include "local/alubox.h"
#include "local/thumb_ldstm.h"

/* **** */

#include "libarm/include/arm_cc.h"
#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <stdlib.h>

/* **** */

static int __thumb_fail_decode(armvm_core_p const core)
{
	for(unsigned lsb = 13; lsb > 8; lsb--) {
		uint32_t group = mlBFTST(IR, 15, lsb);
		LOG("IP[15:%2u]: 0x%08x", lsb, group);
	}

	arm_disasm_thumb(IP, IR);
	LOG_ACTION(return(-1));
}

static int _armvm_core_thumb_add_sub_rn_rd__rm(armvm_core_p const core,
	const int bit_i)
{
	const uint8_t op2 = BEXT(IR, 9);

	setup_rRml(core, ARMVM_TRACE_R(N), 5, 3);
	core_reg_dst_decode(core, ARMVM_TRACE_R(D), 2, 0);

	const unsigned op_list[2] = { ARM_ADD, ARM_SUB };
	const unsigned opcode = op_list[op2];

	alubox_thumb(core, opcode, 1);

	if(!__trace_start(core))
		return(1);

	if(bit_i)
	{
		if(op2 || vR(M)) {
			_armvm_trace_(core, "%ss(%s, %s, %01u)",
				arm_dp_inst_string[opcode], rR_NAME(D), rR_NAME(N), vR(M));
			_armvm_trace_comment(core, "0x%08x %s%01u = 0x%08x",
				vR(N), arm_dp_op_string[opcode], vR(M), vR(D));
		}
		else // pseudo op mov is encoded as adds rd, rn, 0
		{
			_armvm_trace_(core, "mov(%s, %s)", rR_NAME(D), rR_NAME(N));
			_armvm_trace_comment(core, "0x%08x", vR(D));
		}
	}
	else
	{
		_armvm_trace_(core, "%ss(%s, %s, %s)",
			arm_dp_inst_string[opcode], rR_NAME(D), rR_NAME(N), rR_NAME(M));
		_armvm_trace_comment(core, "0x%08x %s0x%08x = 0x%08x",
			vR(N), arm_dp_op_string[opcode], vR(M), vR(D));
	}

	__trace_end(core);
	return(1);
}

static int _armvm_core_thumb_add_sub_rn_rd_imm3(armvm_core_p const core)
{
	(void)setup_vRml(core, ARMVM_TRACE_R(M), 8, 6);

	return(_armvm_core_thumb_add_sub_rn_rd__rm(core, 1));
}

static int _armvm_core_thumb_add_sub_sp_i7(armvm_core_p const core)
{
	const int sub = BEXT(IR, 7);
	const uint16_t imm7 = mlBFMOV(IR, 6, 0, 2);

	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARMVM_GPR(SP));

	uint32_t rd = rn + (sub ? -imm7 : imm7);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARMVM_GPR(SP), rd);

	if(_armvm_trace_start(core, "%s(rSP, 0x%04x)",
			arm_dp_inst_string[sub ? ARM_SUB : ARM_ADD], imm7)) {
		_armvm_trace_comment(core, "0x%08x %s0x%04x = 0x%08x",
			rn, imm7, rd);

		__trace_end(core);
	}

	return(1);
}

static int _armvm_core_thumb_ascm_rd_i(armvm_core_p const core)
{
	const uint8_t opcode = mlBFEXT(IR, 12, 11);

	const uint32_t rm = setup_vRml(core, ARMVM_TRACE_R(M), 7, 0);
	setup_rRml(core, ARMVM_TRACE_R(N), 10, 8);

	setup_rR(core, ARMVM_TRACE_R(D), rR(N));

	const unsigned op_list[4] = {
		ARM_MOV, ARM_CMP, ARM_ADD, ARM_SUB
	}, operation = op_list[opcode];

	const uint32_t rd = alubox_thumb(core, operation, 1);

	if(!__trace_start(core))
		return(1);

	switch(operation)
	{
		default:
			_armvm_trace_(core, "%s(%s, 0x%03x)",
				arm_dp_inst_string[operation], rR_NAME(D), rm);
			_armvm_trace_comment(core, "0x%08x %s0x%03x = 0x%08x",
					vR(N), arm_dp_op_string[operation], rm, rd);
		break;
		case ARM_MOV:
			_armvm_trace_(core, "movs(%s, 0x%03x)", rR_NAME(D), rm);
		break;
	}

	__trace_end(core);

	return(1);
}

static int _armvm_core_thumb_bcc(armvm_core_p const core)
{
	const uint8_t cond = mlBFEXT(IR, 11, 8);
	const int32_t imm8 = mlBFMOVs(IR, 7, 0, 1);

	const uint32_t new_pc = (THUMB_PC_NEXT + imm8) & ~1U;

	if(armvm_core_check_cc(core, cond))
		PC = new_pc;

	if(_armvm_trace_start(core, "b(0x%08x)", new_pc)) {
		_armvm_trace_comment(core, "0x%08x + 0x%03x",
			THUMB_PC_NEXT, imm8);

		__trace_end(core);
	}

	return(0);
}

static int _armvm_core_thumb_bxx__bl_blx(armvm_core_p const core,
	const uint32_t eao, const unsigned blx)
{
	const uint32_t new_pc = LR + eao;

	if(0) LOG("LR = 0x%08x, PC = 0x%08x", LR, PC);

	LR = PC | 1;

//	int splat = _trace_bx_0 && (new_pc == THUMB_PC_NEXT);
	int splat = (new_pc == THUMB_PC_NEXT);
	_armvm_trace(core, "bl%s(0x%08x); /* 0x%08x + %s0x%08x, LR = 0x%08x */",
		blx ? "x" : "", new_pc & ~1U, PC, splat ? "x" : "", eao, LR & ~1U);

	PC = new_pc;
//	soc_core_reg_set_thumb(core, 1 >> blx);

	if(0) LOG("LR = 0x%08x, PC = 0x%08x", LR, PC);
	return(0);
}

static int _armvm_core_thumb_bxx_b(armvm_core_p const core)
{
	const int16_t eao = mlBFMOVs(IR, 10, 0, 1);
	const uint32_t new_pc = THUMB_PC_NEXT + eao;

//	int splat = _trace_bx_0 && (new_pc == THUMB_IP_NEXT);
	int splat = (new_pc == THUMB_IP_NEXT);

	if(_armvm_trace_start(core, "b(0x%08x)", new_pc & ~1U)) {
		_armvm_trace_comment(core, "0x%08x + %s0x%03x",
			PC, splat ? "x" : "", eao);

		__trace_end(core);
	}

	PC = new_pc & ~1U;
	return(0);
}

static int _armvm_core_thumb_bxx_bl(armvm_core_p const core)
{
	const uint32_t eao = mlBFMOV(IR, 10, 0, 1);

	return(_armvm_core_thumb_bxx__bl_blx(core, eao, 0));
}

static int _armvm_core_thumb_bxx_prefix(armvm_core_p const core)
{
	const int32_t eao_prefix = mlBFMOVs(IR, 10, 0, 12);
	const uint8_t h_prefix = mlBFEXT(IR, 12, 11);

	if(2 == h_prefix) {
		LR = THUMB_PC_NEXT + eao_prefix;
	} else {
		LOG_ACTION(return(__thumb_fail_decode(core)));
//		DECODE_FAULT;
	}

	const uint32_t ir_suffix = 0;

	if(0 > armvm_core_mem_ifetch(core, (void*)&ir_suffix, PC & ~1U, sizeof(uint16_t)))
		goto not_prefix_suffix;

	if(0xe800 == (ir_suffix & 0xe800)) {
		const int blx = 1 ^ BEXT(ir_suffix, 12);

		if(blx && (ir_suffix & 1))
			goto not_prefix_suffix; /* undefined instruction */

		IR = (IR << 16) | ir_suffix;
		PC = THUMB_PC_NEXT;

		const uint32_t eao_suffix = mlBFMOV(IR, 10, 0, 1);

		return(_armvm_core_thumb_bxx__bl_blx(core, eao_suffix, blx));
	}

not_prefix_suffix:
	_armvm_trace(core, "BL/BLX(0x%08x)  /* LR = 0x%08x */", eao_prefix, LR);
	return(0);
}

static int _armvm_core_thumb_dp_rms_rdn(armvm_core_p const core)
{
	const uint8_t operation = mlBFEXT(IR, 9, 6);
	const unsigned op_list[16] = {
		ARM_AND, ARM_EOR, ARM_LSL, ARM_LSR, ARM_ASR, ARM_ADC, ARM_SBC, ARM_ROR,
		ARM_TST, ARM_NEG, ARM_CMP, ARM_CMN, ARM_ORR, ARM_MUL, ARM_BIC, ARM_MVN,
	};
	const unsigned opcode = op_list[operation];

	const char* _dpr_ops[2][16] = {{
		"ands", "eors", "lsls", "lsrs", "asrs", "adcs", "sbcs", "rors",
		"tsts", "negs", "cmps", "cmns", "orrs", "muls", "bics", "mvns",
		} , {
		"& ",	"^ ",	"<< ",	">> ",	"<<< ",	"+",	"-",	">><<",
		"& ",	"- ",	"- ",	"+ ",	"| ",	"* ",	"& ~",	"-",
		}};

	const uint32_t rm = core_reg_src_decode(core, ARMVM_TRACE_R(M), 5, 3);
	setup_rRml(core, ARMVM_TRACE_R(N), 2, 0);

	setup_rR(core, ARMVM_TRACE_R(D), rR(N));

	const uint32_t rd = alubox_thumb(core, opcode, 1);

	if(_armvm_trace_start(core, "%s(%s, %s)",
		_dpr_ops[0][operation], rR_NAME(D), rR_NAME(M))) {

		switch(opcode)
		{
			default:
				_armvm_trace_comment(core, "0x%08x %s0x%08x = 0x%08x",
					vR(N), _dpr_ops[1][operation], rm, rd);
				break;
			case ARM_MVN:
				_armvm_trace_comment(core, "~0x%08x = 0x%08x",
					rm, rd);
				break;
		}

		__trace_end(core);
	}

	return(1);
}

static int _armvm_core_thumb_ldst_rd_i(armvm_core_p const core)
{
	const uint16_t operation = mlBFTST(IR, 15, 12);
	const int bit_l = BEXT(IR, 11);
	const uint16_t imm8 = mlBFMOV(IR, 7, 0, 2);

	setup_rRml(core, ARMVM_TRACE_R(D), 10, 8);

	uint32_t rn;
	switch(operation)
	{
		case	0x4000:
			rn = setup_rR_vR(core, ARMVM_TRACE_R(N), ARMVM_GPR(PC), PC & ~3U);
			break;
		case	0x9000:
			rn = setup_rR_vR(core, ARMVM_TRACE_R(N), ARMVM_GPR(SP), SP);
			break;
		default:
			LOG("operation = 0x%03x", operation);
			arm_disasm_thumb(IP, IR);
			LOG_ACTION(exit(1));
			break;
	}

	const uint32_t ea = setup_vR(core, ARMVM_TRACE_R(EA), rn + imm8);
	int read_rval = 0;

	if(bit_l) {
		read_rval = armvm_core_mem_read(core, &vR(D), ea, 4);
		if(1 == read_rval)
			core_reg_wb(core, ARMVM_TRACE_R(D));
	} else {
		core_reg_src_load(core, ARMVM_TRACE_R(D));
		armvm_core_mem_write(core, ea, 4, vR(D));
	}

	if(__trace_start(core)) {
		_armvm_trace_(core, "%s(%s, %s[0x%03x])",
			bit_l ? "ldr" : "str", rR_NAME(D), rR_NAME(N), imm8);

		if(!bit_l || (1 == read_rval))
			_armvm_trace_comment(core, "[0x%08x](0x%08x)",
				ea, vR(D));

		__trace_end(core);
	}

	return(1);
}

static int _armvm_core_thumb_ldstm_rn_rxx(armvm_core_p const core)
{
//	struct {
		const int bit_l = BEXT(IR, 11);
//	}bit;

	const uint32_t rn = core_reg_src_decode(core, ARMVM_TRACE_R(N), 10, 8);

	const uint8_t rlist = mlBFEXT(IR, 7, 0);

	const uint8_t _rcount = (uint8_t)__builtin_popcount(rlist);
	const uint8_t rcount_bytes = _rcount << 2;

	const uint32_t start_address = vR(N);
	const uint32_t end_address = start_address + rcount_bytes - 4;

	uint32_t ea = start_address;

	/* CP15_r1_Ubit == 0 */
	if(start_address & 3) {
		if(CP15_REG1_BIT(A) || CP15_REG1_BIT(U))
			return(armvm_core_exception_data_abort(core));

		ea &= ~3;
	}

	char reglist[9] = "\0\0\0\0\0\0\0\0\0";

	for(unsigned i = 0; i <= 7; i++)
	{
		const unsigned rxx = BEXT(rlist, i);
		reglist[i] = rxx ? ('0' + i) : '.';

		if(rxx)
		{
			CYCLE++;

			if(bit_l)
				thumb_ldmia(core, i, &ea);
			else
				thumb_stmia(core, i, &ea);
		}
	}

	assert(end_address == ea - 4);

	const int wb_l = bit_l && (0 == BTST(rlist, rR(N)));
	const int wb = !bit_l || wb_l;

	if(wb)
		core_reg_wb_v(core, ARMVM_TRACE_R(N), ea);

	reglist[8] = 0;

	if(_armvm_trace_start(core, "%smia(%s%s, r{%s})",
			bit_l ? "ld" : "st", rR_NAME(N),
			wb ? "!" : "", reglist)) {
		_armvm_trace_comment(core, "0x%08x", rn);

		__trace_end(core);
	}

	return(1);
}

static int _armvm_core_thumb_sbi_imm5_rm_rd(armvm_core_p const core)
{
	const uint32_t shift_type = mlBFEXT(IR, 12, 11);
	const uint32_t rs = setup_vR(core, ARMVM_TRACE_R(S), mlBFEXT(IR, 10, 6));

	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), mlBFEXT(IR, 5, 3));

	const uint32_t rd = arm_shiftbox(shift_type, rm, rs, IF_CPSR(C));
	ARM_CPSR_BMAS(C, arm_shiftbox_c(shift_type, rm, rs));

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), mlBFEXT(IR, 2, 0), rd);

	const char* sops = arm_sop_lcase_string[shift_type];

	if(_armvm_trace_start(core, "%ss(%s, %s, 0x%02x)",
			sops, rR_NAME(D), rR_NAME(M), rs)) {
		_armvm_trace_comment(core, "%s(0x%08x, 0x%02x) = 0x%08x",
			sops, rm, rs, rd);

		__trace_end(core);
	}

	return(1);
}

static int armvm_core_thumb__step_group0_0000_1fff(armvm_core_p const core)
{
	switch(mlBFTST(IR, 15, 10)) {
		case 0x1800: /* 0001 10xx xxxx xxxx */
break;//			return(soc_core_thumb_add_sub_rn_rd_rm(core));
		case 0x1c00: /* 0001 11xx xxxx xxxx */
			return(_armvm_core_thumb_add_sub_rn_rd_imm3(core));
		default:
			return(_armvm_core_thumb_sbi_imm5_rm_rd(core));
	}

	LOG_ACTION(return(__thumb_fail_decode(core)));
}

static int armvm_core_thumb__step_group2_4000_5fff(armvm_core_p const core)
{
	if(0x5000 == mlBFTST(IR, 15, 12)) { /* 0101 xxxx xxxx xxxx */
;//		return(soc_core_thumb_ldst_rm_rn_rd(core));
	} else if(0x4800 == mlBFTST(IR, 15, 11)) { /* 0100 1xxx xxxx xxxx */
		return(_armvm_core_thumb_ldst_rd_i(core));
	} else {
		switch(mlBFTST(IR, 15, 10)) {
			case 0x4000: /* 0100 00xx xxxx xxxx */
				return(_armvm_core_thumb_dp_rms_rdn(core));
			case 0x4400: /* 0100 01xx xxxx xxxx */
				switch(mlBFTST(IR, 15, 8)) {
					case 0x4700: /* 0100 0111 xxxx xxxx */
break;//						return(soc_core_thumb_bx(core));
					default: /* 0100 01xx xxxx xxxx */
break;//						return(soc_core_thumb_sdp_rms_rdn(core));
				}
				break;
		}
	}

	LOG_ACTION(return(__thumb_fail_decode(core)));
}

static int armvm_core_thumb__step_group5_b000_bfff(armvm_core_p const core)
{
	switch(mlBFTST(IR, 15, 8)) {
		case 0xb000: /* 1011 0000 xxxx xxxx */
			return(_armvm_core_thumb_add_sub_sp_i7(core));
		case 0xb400: /* 1011 0100 xxxx xxxx */
		case 0xb500: /* 1011 0101 xxxx xxxx */
		case 0xbc00: /* 1011 1100 xxxx xxxx */
		case 0xbd00: /* 1011 1101 xxxx xxxx */
break;//			return(soc_core_thumb_pop_push(core));
	}

	LOG_ACTION(return(__thumb_fail_decode(core)));
}

static int armvm_core_thumb__step_group6_c000_dfff(armvm_core_p const core)
{
	if(BTST(IR, 12)) {
		switch(mlBFTST(IR, 15, 8)) {
			case 0xde00: /* 1101 1110 xxxx xxxx -- undefined */
				return(armvm_core_exception_undefined_instruction(core));
			case 0xdf00: /* 1101 1111 xxxx xxxx -- swi */
				return(armvm_core_exception_swi(core));
			default: /* 1101 xxxx xxxx xxxx */
				return(_armvm_core_thumb_bcc(core));
		}
	} else { /* 1100 xxxx xxxx xxxx */
		return(_armvm_core_thumb_ldstm_rn_rxx(core));
	}

	LOG_ACTION(return(__thumb_fail_decode(core)));
}

static int armvm_core_thumb__step_group7_e000_ffff(armvm_core_p const core)
{
	switch(mlBFTST(IR, 15, 11)) {
		case 0xe000: /* 1110 0xxx xxxx xxxx */
			return(_armvm_core_thumb_bxx_b(core));
		case 0xe800:
			if(IR & 1) { /* 1110 1xxx xxxx xxx1 */
break;//				LOG_ACTION(return(soc_core_thumb_step_undefined(core)));
			} else /* 1110 1xxx xxxx xxx0 */
break;//				return(soc_core_thumb_bxx_blx(core));
		case 0xf800:
			return(_armvm_core_thumb_bxx_bl(core));
		case 0xf000:
			return(_armvm_core_thumb_bxx_prefix(core));
	}

	LOG_ACTION(return(__thumb_fail_decode(core)));
}

int armvm_core_thumb_step(armvm_core_p const core)
{
	rSPR32(CC) = CC_AL_NV;
	CCX = 1;

	IP = setup_rR_vR(core, ARMVM_TRACE_R(IP), ~0, PC); // STUPID KLUDGE!!
	PC = THUMB_IP_NEXT;

	if(0 > armvm_core_mem_ifetch(core, &IR, IP & ~1U, 2))
		return(0);

	setup_rR_vR(core, ARMVM_TRACE_R(IR), ~0, IR); // STUPID KLUDGE!!!

	const uint32_t group = mlBFTST(IR, 15, 13);
	switch(group) {
		case 0x0000: /* 000x xxxx xxxx xxxx */
			return(armvm_core_thumb__step_group0_0000_1fff(core));
		case 0x2000: /* 001x xxxx xxxx xxxx */
			return(_armvm_core_thumb_ascm_rd_i(core));
		case 0x4000: /* 010x xxxx xxxx xxxx */
			return(armvm_core_thumb__step_group2_4000_5fff(core));
		case 0xa000: /* 101x xxxx xxxx xxxx */
			if(BTST(IR, 12)) /* 1011 xxxx xxxx xxxx */
				return(armvm_core_thumb__step_group5_b000_bfff(core));
			else /* 1010 xxxx xxxx xxxx */
break;//				return(soc_core_thumb_add_rd_pcsp_i(core));
			break;
		case 0xc000: /* 110x xxxx xxxx xxxx */
			return(armvm_core_thumb__step_group6_c000_dfff(core));
		case 0xe000: /* 111x xxxx xxxx xxxx */
			return(armvm_core_thumb__step_group7_e000_ffff(core));
		default:
			LOG("IP[15:13]: 0x%08x", group);
	}

	return(__thumb_fail_decode(core));
}
