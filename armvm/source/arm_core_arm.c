#include "armvm_core_alubox.h"
#include "armvm_core_cc.h"
#include "armvm_core_glue.h"
#include "armvm_core_ldst.h"
#include "armvm_core_reg_trace.h"
#include "armvm_core_shiftbox.h"
#include "armvm_core_trace.h"

/* **** */

#include "armvm_config.h"
#include "armvm.h"

/* **** */

#include "arm_trace_arm.h"
#include "arm_trace.h"

#include "arm_exception.h"
#include "arm_mem.h"

/* **** */

#include "arm_cc.h"
#include "arm_cpsr.h"
#include "arm_disasm.h"
#include "arm_dp.h"
#include "arm_ir.h"
#include "arm_shiftbox.h"
#include "arm_strings.h"

/* **** */

#include "log.h"

/* **** */

#include <assert.h>
#include <ctype.h>
#include <string.h>

/* **** */

static void __arm_decode_fail(armvm_p avm)
{
	LOG("group: %01u", ARM_IR_GROUP);

	switch(ARM_IR_GROUP) {
		case 2: {

			char *ipubwl = "ipubwl", out[7], *dst = out;

			for(unsigned x = 0; x < 6 ; x++) {
				char c = ipubwl[x];
				*dst++ = BEXT(ARM_IR_LDST_IPUBWL, 5 - x) ? toupper(c) : c;
			}
			*dst = 0;

			LOG("0x%02x -- %s", ARM_IR_LDST_IPUBWL, out);
		}
		break;
	}

	arm_disasm(IP, IR);
	LOG_ACTION(exit(-1));
}

static void __arm__b_bl_blx(int link, int32_t offset, int x, armvm_p avm)
{
	if(CCX) {
		if(link) {
			CYCLE++;
			LR = PC;
		}

		CYCLE++;
		PC = ARM_PC_NEXT + offset;
	}

	/* **** */

	arm_trace_b_bl(pARM_TRACE);

	/* **** */
}

/* **** */

static void _arm_inst_b_bl(armvm_p avm)
{
	return(__arm__b_bl_blx(ARM_IR_B_LINK, ARM_IR_B_OFFSET, 0, avm));
}

static void _arm_inst_blx(armvm_p avm)
{
	return(__arm__b_bl_blx(1, ARM_IR_BLX_OFFSET, 1, avm));
}

static void _arm_inst_bx_blx_m(int link, armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_RD);
		assert(15 == ARM_IR_RN);
		assert(15 == ARM_IR_RS);
	}

	const uint32_t rm = arm_reg_src(avm, ARM_TRACE_R(M), ARM_IR_R(M));
	const unsigned thumb = avm->arm_config.features.thumb ? (rm & 1) : 0;

	if(CCX) {
		CYCLE++;

		if(link) {
			CYCLE++;
			LR = PC;
		}

		PC = rm & (~1 << (1 >> thumb));

		ARM_CPSR_BMAS(T, thumb);
	}

	if(pARM_TRACE) { // TODO
		_arm_trace(pARM_TRACE, "b%sx, %s", link ? "l" :"", rR(M));
		_arm_trace_comment(pARM_TRACE, "%s(0x%08x)", thumb ? "T" : "A", vR(M));
	}
}

static void _arm_inst_clz(armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(15 == ARM_IR_R(S));
	}

	const uint32_t rn = arm_reg_src(avm, ARM_TRACE_R(N), ARM_IR_R(N));
	uint32_t rd = rn ? 31 : 0;

	if(rn) {
//		while(rd && rn) { rd--; rn >>= 1; }
		while(rd && !BEXT(rn, rd)) rd--;
	}

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);

	if(pARM_TRACE)
		LOG_ACTION(exit(-1));
}

typedef void (*arm_dp_fn)(uint32_t sop, armvm_p avm);

static void _alubox_(arm_dp_fn fn, uint32_t sop, armvm_p avm)
{
	fn(sop, avm);

	if(pARM_TRACE)
		arm_trace_dp(pARM_TRACE);
}

static void _arm_inst_dp(uint32_t sop, unsigned carry_in, armvm_p avm)
{
	if(ARM_IR_DP_S) {
		switch(ARM_IR_DP_OPCODE) {
		case ARM_ADC: return(_alubox_(alubox_adcs, sop, avm));
		case ARM_ADD: return(_alubox_(alubox_adds, sop, avm));
		case ARM_AND: return(_alubox_(alubox_ands, sop, avm));
		case ARM_BIC: return(_alubox_(alubox_bics, sop, avm));
		case ARM_CMN: return(_alubox_(alubox_cmns, sop, avm));
		case ARM_CMP: return(_alubox_(alubox_cmps, sop, avm));
		case ARM_EOR: return(_alubox_(alubox_eors, sop, avm));
		case ARM_MOV: return(_alubox_(alubox_movs, sop, avm));
		case ARM_MVN: return(_alubox_(alubox_mvns, sop, avm));
		case ARM_ORR: return(_alubox_(alubox_orrs, sop, avm));
		case ARM_RSB: return(_alubox_(alubox_rsbs, sop, avm));
		case ARM_RSC: return(_alubox_(alubox_rscs, sop, avm));
		case ARM_SUB: return(_alubox_(alubox_subs, sop, avm));
		case ARM_TEQ: return(_alubox_(alubox_teqs, sop, avm));
		case ARM_TST: return(_alubox_(alubox_tsts, sop, avm));
		default:
			LOG_ACTION(return(__arm_decode_fail(avm)));
			LOG_ACTION(exit(-1));
		}
	} else {
		switch(ARM_IR_DP_OPCODE) {
		case ARM_ADC: return(_alubox_(alubox_adc_wb, sop, avm));
		case ARM_ADD: return(_alubox_(alubox_add_wb, sop, avm));
		case ARM_AND: return(_alubox_(alubox_and_wb, sop, avm));
		case ARM_BIC: return(_alubox_(alubox_bic_wb, sop, avm));
		case ARM_EOR: return(_alubox_(alubox_eor_wb, sop, avm));
		case ARM_MOV: return(_alubox_(alubox_mov_wb, sop, avm));
		case ARM_MVN: return(_alubox_(alubox_mvn_wb, sop, avm));
		case ARM_ORR: return(_alubox_(alubox_orr_wb, sop, avm));
		case ARM_RSB: return(_alubox_(alubox_rsb_wb, sop, avm));
		case ARM_RSC: return(_alubox_(alubox_rsc_wb, sop, avm));
		case ARM_SBC: return(_alubox_(alubox_sbc_wb, sop, avm));
		case ARM_SUB: return(_alubox_(alubox_sub_wb, sop, avm));
		default:
			LOG_ACTION(return(__arm_decode_fail(avm)));
			LOG_ACTION(exit(-1));
		}
	}

	LOG_ACTION(return(__arm_decode_fail(avm)));
}

static void _arm_inst_dp_immediate(armvm_p avm)
{
	const unsigned carry_in = IF_CPSR(C);

	const uint32_t rm = setup_rR_vR(avm, ARM_TRACE_R(M), ~0, ARM_IR_DPI_IMMEDIATE);
	const uint32_t rs = setup_rR_vR(avm, ARM_TRACE_R(S), ~0, ARM_IR_DPI_ROTATE_AMOUNT);

if(0) LOG("rm: 0x%08x, rs: 0x%08x, carry_in: 0x%08x", rm, rs, carry_in);

	const uint32_t sop = arm_shiftbox(ARM_SOP_ROR,
		rm, rs, carry_in);

if(0) LOG("sop: 0x%08x", sop);

	setup_rR_vR(avm, ARM_TRACE_R(SOP), ~0, sop);

	return(_arm_inst_dp(sop, carry_in, avm));
}

static void _arm_inst_dp_shift(uint32_t rs, armvm_p avm)
{
	if(CCX) CYCLE++;

	const uint32_t rm = arm_reg_src(avm, ARM_TRACE_R(M), ARM_IR_R(M));

	const unsigned carry_in = IF_CPSR(C);
	const uint32_t sop = arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE,
		rm, rs, carry_in);

	setup_rR_vR(avm, ARM_TRACE_R(SOP), ~0, sop);

	return(_arm_inst_dp(sop, carry_in, avm));
}

static void _arm_inst_dp_shift_register(armvm_p avm)
{
	if(CCX) CYCLE++;

	const uint32_t rs = arm_reg_src(avm, ARM_TRACE_R(S), ARM_IR_R(S));

	return(_arm_inst_dp_shift(rs, avm));
}

static void _arm_inst_dp_shift_immediate(armvm_p avm)
{
	const uint32_t rs = setup_rR_vR(avm, ARM_TRACE_R(S), ~0, ARM_IR_DP_SHIFT_AMOUNT);

	return(_arm_inst_dp_shift(rs, avm));
}

static void _arm_inst_ldst(uint32_t sop, armvm_p avm)
{
	if(ARM_IR_LDST_BIT(L)) {
		if(ARM_IR_LDST_BIT(B))
			__ldst_ldrb(sop, avm);
		else
			__ldst_ldr(sop, avm);
	} else {
		if(ARM_IR_LDST_BIT(B))
			__ldst_strb(sop, avm);
		else
			__ldst_str(sop, avm);
	}

	arm_trace_ldst(pARM_TRACE);
}

static void _arm_inst_ldst_immediate(armvm_p avm)
{
	const uint32_t rm = setup_rR_vR(avm, ARM_TRACE_R(M), ~0, ARM_IR_LDST_IMMEDIATE_OFFSET);
	const uint32_t sop = setup_rR_vR(avm, ARM_TRACE_R(SOP), ~0, rm);

	return(_arm_inst_ldst(sop, avm));
}

static void _arm_inst_ldst_sh(uint32_t rm, armvm_p avm)
{
	const unsigned bwh  = BMOV(IR, ARM_IR_LDST_BIT_L, 2) | mlBFEXT(IR, 6, 5);

	switch(bwh) {
		case 1: __ldst_strh(rm, avm); break;
//		case 2: __ldst_ldrd(rm, avm); break;
//		case 3: __ldst_strd(rm, avm); break;
		case 5: __ldst_ldrh(rm, avm); break;
		case 6: __ldst_ldrsb(rm, avm); break;
		case 7: __ldst_ldrsh(rm, avm); break;
		default:
			UNDEFINED;
			break;
	}

	arm_trace_ldst(pARM_TRACE);
}

static void _arm_inst_ldst_sh_immediate(armvm_p avm)
{
	const uint rm = setup_rR_vR(avm, ARM_TRACE_R(M), ~0, ARM_IR_LDST_SH_OFFSET);
	return(_arm_inst_ldst_sh(rm, avm));
}

static void _arm_inst_ldst_sh_register(armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RS);

	const uint rm = arm_reg_src(avm, ARM_TRACE_R(M), ARM_IR_R(M));
	return(_arm_inst_ldst_sh(rm, avm));
}

static void _arm_inst_mla(armvm_p avm)
{
	const uint32_t rn = arm_reg_src(avm, ~0, ARM_IR_R(N));
	const uint32_t rm = arm_reg_src(avm, ~0, ARM_IR_R(M));
	const uint32_t rs = arm_reg_src(avm, ~0 , ARM_IR_R(S));

	const uint32_t rd = (rm * rs) + rn;

	arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), rd);

	if(CCX && ARM_IR_DP_S)
		_alubox_flags__nz(rGPR(D), avm);

	/* **** */

	if(pARM_TRACE)
		arm_trace_mla(pARM_TRACE);
}

static void _arm_inst_msr(armvm_p avm)
{
	if(vmCONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(0 == ARM_IR_SHIFT_OPERAND);
	}

	if(ARM_IR_MSR_R) {
		if(pSPSR)
			arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), SPSR);
	} else
		arm_reg_dst_wb(avm, ARM_TRACE_R(D), ARM_IR_R(D), CPSR);

	/* **** */

	if(pARM_TRACE)
		arm_trace_msr(pARM_TRACE);
}

static void _arm_inst_umull(armvm_p avm)
{
	const uint32_t rs = arm_reg_src(avm, ARM_TRACE_R(S), ARM_IR_R(S));
	const uint32_t rm = arm_reg_src(avm, ARM_TRACE_R(M), ARM_IR_R(M));

	rSPR64(RESULT) = (uint32_t)rm * (uint32_t)rs;

	const uint32_t lo = rSPR64lo(RESULT);
	const uint32_t hi = rSPR64hi(RESULT);

	arm_reg_dst_wb(avm, ARM_TRACE_R(DLo), ARM_IR_R(DLo), lo);
	arm_reg_dst_wb(avm, ARM_TRACE_R(DHi), ARM_IR_R(DHi), hi);

	if(CCX && ARM_IR_DP_S) {
		ARM_CPSR_BMAS(N, BEXT(hi, 31));
		ARM_CPSR_BMAS(Z, (0 == rSPR64(RESULT)));
	}

	/* **** */

	if(pARM_TRACE)
		arm_trace_umull(pARM_TRACE);
}

/* **** */

static void armvm_step0_ldst(armvm_p avm)
{
	if(9 != mlBFTST(IR, 7, 4)) {
		if(ARM_IR_LDST_SH_BIT(I))
			return(_arm_inst_ldst_sh_immediate(avm));
		else
			return(_arm_inst_ldst_sh_register(avm));
	}

	return(__arm_decode_fail(avm));
}

static void armvm_step0_misc(armvm_p avm)
{
	switch(mlBFTST(IR, 27, 20) | mlBFTST(IR, 7, 4)) {
//		case 0x00000090:
//		case 0x00100090: return(_arm_inst_mul(avm));
		case 0x00200090:
		case 0x00300090: return(_arm_inst_mla(avm));
		case 0x00800090:
		case 0x00900090: return(_arm_inst_umull(avm));
		case 0x01200030: return(_arm_inst_bx_blx_m(1, avm));
		case 0x01200010: return(_arm_inst_bx_blx_m(0, avm));
		case 0x01000000:
		case 0x01400000: return(_arm_inst_msr(avm));
		case 0x01600010: return(_arm_inst_clz(avm));
	}

	return(__arm_decode_fail(avm));
}

static void armvm_step0(armvm_p avm)
{
	if(BEXT(IR, 4)) {
		if(BEXT(IR, 7))
			return(armvm_step0_ldst(avm));
		else if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(armvm_step0_misc(avm));
		else
			return(_arm_inst_dp_shift_register(avm));

	} else {
		if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(armvm_step0_misc(avm));
		else
			return(_arm_inst_dp_shift_immediate(avm));
	}

	return(__arm_decode_fail(avm));
}

static void armvm_step1_misc(armvm_p avm)
{
	return(__arm_decode_fail(avm));
}

static void armvm_step1(armvm_p avm)
{
	if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
		return(armvm_step1_misc(avm));
	else
		return(_arm_inst_dp_immediate(avm));

	return(__arm_decode_fail(avm));
}

void armvm_step_arm(armvm_p avm)
{
	IP = setup_rR_vR(avm, ARM_TRACE_R(IP), ~0, PC); // STUPID KLUDGE!!
	PC = ARM_IP_NEXT;

	if(0 > arm_mem_ifetch(&IR, IP, 4, avm))
		return;

	setup_rR_vR(avm, ARM_TRACE_R(IR), ~0, IR); // STUPID KLUDGE!!!

	armvm_core_check_cc(ARM_IR_CC, avm);

	switch(ARM_IR_CC) {
	default:
		switch(ARM_IR_GROUP) {
			case 0: // xxxx 000x xxxx xxxx
				return(armvm_step0(avm));
			case 1: // xxxx 001x xxxx xxxx
				return(armvm_step1(avm));
			case 2: // xxxx 010x xxxx xxxx
				return(_arm_inst_ldst_immediate(avm));
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_b_bl(avm));
			default:
				return(__arm_decode_fail(avm));
		}
		break;
	case CC_NV:
		switch(ARM_IR_GROUP) {
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_blx(avm));
			default:
				return(__arm_decode_fail(avm));
		}
		break;
	}

	return(__arm_decode_fail(avm));
}
