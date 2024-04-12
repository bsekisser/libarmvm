#include "armvm_core_alubox.h"
#include "armvm_core_cc.h"
#include "armvm_core_config.h"
//#include "armvm_core_exception.h"
#include "armvm_core_glue.h"
#include "armvm_core_ldst.h"
//#include "armvm_core_mem.h"
#include "armvm_core_reg_trace.h"
#include "armvm_core_shiftbox.h"
#include "armvm_core_trace.h"

/* **** */

#include "armvm_mmu.h"
#include "armvm.h"

/* **** */

#include "armvm_trace_arm.h"
#include "armvm_trace.h"

/* **** */

#include "libarm/include/arm_cc.h"
#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_shiftbox.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <assert.h>
#include <ctype.h>
#include <string.h>

/* **** */

static void __arm_decode_fail(armvm_core_p const core)
{
	LOG("group: %01u", ARM_IR_GROUP);

	switch(ARM_IR_GROUP) {
		case 2: {

			char *ipubwl = "ipubwl", out[7], *dst = out;

			for(unsigned x = 0; x < 6 ; x++) {
				const char c = ipubwl[x];
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

static void __arm__b_bl_blx(int const link, int x, int32_t const offset, armvm_core_p const core)
{
	if(CCX) {
		if(link) {
//			CYCLE++;
			LR = PC;
		}

		CYCLE++;
		PC = ARM_PC_NEXT + offset;

		if(x) ARM_CPSR_BSET(Thumb);
	}

	/* **** */

	if(pARMVM_TRACE) {
		if(x)
			armvm_trace_blx(pARMVM_TRACE);
		else
			armvm_trace_b_bl(pARMVM_TRACE);
	}

	/* **** */
}

/* **** */

static void _arm_inst_b_bl(armvm_core_p const core)
{
	return(__arm__b_bl_blx(ARM_IR_B_LINK, 0, ARM_IR_B_OFFSET, core));
}

static void _arm_inst_blx(armvm_core_p const core)
{
	CCX = 1; rSPR32(CC) = CC_AL;
	return(__arm__b_bl_blx(1, 1, ARM_IR_BLX_OFFSET, core));
}

static void _arm_inst_bx_blx_m(int const link, armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_RD);
		assert(15 == ARM_IR_RN);
		assert(15 == ARM_IR_RS);
	}

	const uint32_t rm = arm_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const unsigned thumb = CONFIG->features.thumb ? (rm & 1) : 0;

	if(CCX) {
		CYCLE++;

		if(link) {
			CYCLE++;
			LR = PC;
		}

		PC = rm & (~1U << (1 >> thumb));

		ARM_CPSR_BMAS(T, thumb);
	}

	if(pARMVM_TRACE) {
		if(link)
			armvm_trace_blx_m(pARMVM_TRACE);
		else
			armvm_trace_bx_m(pARMVM_TRACE);
	}
}

static void _arm_inst_clz(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(15 == ARM_IR_R(S));
	}

	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	uint32_t rd = rn ? 31 : 0;

	if(rn) {
//		while(rd && rn) { rd--; rn >>= 1; }
		while(rd && !BEXT(rn, rd)) rd--;
	}

	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);

	if(pARMVM_TRACE) // TODO
		LOG_ACTION(exit(-1));
}

typedef void (*arm_dp_fn)(uint32_t const sop, armvm_core_p const core);

static void _alubox_(arm_dp_fn const fn, uint32_t const sop, armvm_core_p const core)
{
	fn(sop, core);

	if(pARMVM_TRACE)
		armvm_trace_dp(pARMVM_TRACE);
}

static void _arm_inst_dp(uint32_t const sop, unsigned const carry_in, armvm_core_p const core)
{
	if(ARM_IR_DP_S) {
		switch(ARM_IR_DP_OPCODE) {
		case ARM_ADC: return(_alubox_(alubox_adcs, sop, core));
		case ARM_ADD: return(_alubox_(alubox_adds, sop, core));
		case ARM_AND: return(_alubox_(alubox_ands, sop, core));
		case ARM_BIC: return(_alubox_(alubox_bics, sop, core));
		case ARM_CMN: return(_alubox_(alubox_cmns, sop, core));
		case ARM_CMP: return(_alubox_(alubox_cmps, sop, core));
		case ARM_EOR: return(_alubox_(alubox_eors, sop, core));
		case ARM_MOV: return(_alubox_(alubox_movs, sop, core));
		case ARM_MVN: return(_alubox_(alubox_mvns, sop, core));
		case ARM_ORR: return(_alubox_(alubox_orrs, sop, core));
		case ARM_RSB: return(_alubox_(alubox_rsbs, sop, core));
		case ARM_RSC: return(_alubox_(alubox_rscs, sop, core));
		case ARM_SBC: return(_alubox_(alubox_sbcs, sop, core));
		case ARM_SUB: return(_alubox_(alubox_subs, sop, core));
		case ARM_TEQ: return(_alubox_(alubox_teqs, sop, core));
		case ARM_TST: return(_alubox_(alubox_tsts, sop, core));
		default:
			LOG_ACTION(return(__arm_decode_fail(core)));
			LOG_ACTION(exit(-1));
		}
	} else {
		switch(ARM_IR_DP_OPCODE) {
		case ARM_ADC: return(_alubox_(alubox_adc_wb, sop, core));
		case ARM_ADD: return(_alubox_(alubox_add_wb, sop, core));
		case ARM_AND: return(_alubox_(alubox_and_wb, sop, core));
		case ARM_BIC: return(_alubox_(alubox_bic_wb, sop, core));
		case ARM_EOR: return(_alubox_(alubox_eor_wb, sop, core));
		case ARM_MOV: return(_alubox_(alubox_mov_wb, sop, core));
		case ARM_MVN: return(_alubox_(alubox_mvn_wb, sop, core));
		case ARM_ORR: return(_alubox_(alubox_orr_wb, sop, core));
		case ARM_RSB: return(_alubox_(alubox_rsb_wb, sop, core));
		case ARM_RSC: return(_alubox_(alubox_rsc_wb, sop, core));
		case ARM_SBC: return(_alubox_(alubox_sbc_wb, sop, core));
		case ARM_SUB: return(_alubox_(alubox_sub_wb, sop, core));
		default:
			LOG_ACTION(return(__arm_decode_fail(core)));
			LOG_ACTION(exit(-1));
		}
	}

	LOG_ACTION(return(__arm_decode_fail(core)));
	UNUSED(carry_in);
}

static void _arm_inst_dp_immediate(armvm_core_p const core)
{
	const unsigned carry_in = IF_CPSR(C);

	const uint32_t rm = setup_rR_vR(core, ARMVM_TRACE_R(M), ~0, ARM_IR_DPI_IMMEDIATE);
	const uint32_t rs = setup_rR_vR(core, ARMVM_TRACE_R(S), ~0, ARM_IR_DPI_ROTATE_AMOUNT);

if(0) LOG("rm: 0x%08x, rs: 0x%08x, carry_in: 0x%08x", rm, rs, carry_in);

	const uint32_t sop = arm_shiftbox(ARM_SOP_ROR,
		rm, rs, carry_in);

if(0) LOG("sop: 0x%08x", sop);

	setup_rR_vR(core, ARMVM_TRACE_R(SOP), ~0, sop);

	return(_arm_inst_dp(sop, carry_in, core));
}

static void _arm_inst_dp_shift(uint32_t const rs, unsigned immediate, armvm_core_p const core)
{
	if(CCX) CYCLE++;

	const uint32_t rm = arm_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));

	const unsigned carry_in = IF_CPSR(C);

	const uint32_t sop = (immediate ? arm_shiftbox_immediate : arm_shiftbox)
		(ARM_IR_DP_SHIFT_TYPE, rm, rs, carry_in);

	setup_rR_vR(core, ARMVM_TRACE_R(SOP), ~0, sop);

	return(_arm_inst_dp(sop, carry_in, core));
}

static void _arm_inst_dp_shift_register(armvm_core_p const core)
{
	if(CCX) CYCLE++;

	const uint32_t rs = arm_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));

	return(_arm_inst_dp_shift(rs, 0, core));
}

static void _arm_inst_dp_shift_immediate(armvm_core_p const core)
{
	const uint32_t rs = setup_rR_vR(core, ARMVM_TRACE_R(S), ~0, ARM_IR_DP_SHIFT_AMOUNT);

	return(_arm_inst_dp_shift(rs, 1, core));
}

static void _arm_inst_ldst(uint32_t const sop, armvm_core_p const core)
{
	if(ARM_IR_LDST_BIT(L)) {
		if(ARM_IR_LDST_BIT(B))
			__ldst_ldrb(sop, core);
		else
			__ldst_ldr(sop, core);
	} else {
		if(ARM_IR_LDST_BIT(B))
			__ldst_strb(sop, core);
		else
			__ldst_str(sop, core);
	}

	armvm_trace_ldst(pARMVM_TRACE);
}

static void _arm_inst_ldst_immediate(armvm_core_p const core)
{
	const uint32_t rm = setup_rR_vR(core, ARMVM_TRACE_R(M), ~0, ARM_IR_LDST_IMMEDIATE_OFFSET);
	const uint32_t sop = setup_rR_vR(core, ARMVM_TRACE_R(SOP), ~0, rm);

	return(_arm_inst_ldst(sop, core));
}

static void _arm_inst_ldst_sh(uint32_t const rm, armvm_core_p const core)
{
	const unsigned bwh  = BMOV(IR, ARM_IR_LDST_BIT_L, 2) | mlBFEXT(IR, 6, 5);

	switch(bwh) {
		case 1: __ldst_strh(rm, core); break;
//		case 2: __ldst_ldrd(rm, core); break;
//		case 3: __ldst_strd(rm, core); break;
		case 5: __ldst_ldrh(rm, core); break;
		case 6: __ldst_ldrsb(rm, core); break;
		case 7: __ldst_ldrsh(rm, core); break;
		default:
			return(__arm_decode_fail(core));;
	}

	armvm_trace_ldst(pARMVM_TRACE);
}

static void _arm_inst_ldst_sh_immediate(armvm_core_p const core)
{
	const uint rm = setup_rR_vR(core, ARMVM_TRACE_R(M), ~0, ARM_IR_LDST_SH_OFFSET);
	return(_arm_inst_ldst_sh(rm, core));
}

static void _arm_inst_ldst_sh_register(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RS);

	const uint rm = arm_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	return(_arm_inst_ldst_sh(rm, core));
}

static void _arm_inst_mcr_mrc(armvm_core_p const core)
{
	armvm_trace_mcr_mrc(pARMVM_TRACE);

	if(!CCX)
		return;

	uint32_t rd = ARM_IR_MCRC_L ? 0 : irGPR(D);
	uint32_t *const p2rd = ARM_IR_MCRC_L ? &irGPR(D) : 0;

	rd = armvm_coprocessor_access(p2rd, core->cp);

	if(ARM_IR_MCRC_L) {
		if(rR_IS_PC(D)) {
			ARM_CPSR_BIC(ARM_CPSR_MASK_NZCV, rd);
		} else
			irGPR(D) = rd;
	}
}

static void _arm_inst_mla(armvm_core_p const core)
{
	const uint32_t rn = arm_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	const uint32_t rm = arm_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const uint32_t rs = arm_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));

	const uint32_t rd = (rm * rs) + rn;

	arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);

	if(CCX && ARM_IR_DP_S)
		_alubox_flags__nz(irGPR(D), core);

	/* **** */

	if(pARMVM_TRACE)
		armvm_trace_mla(pARMVM_TRACE);
}

static void _arm_inst_msr(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(0 == ARM_IR_SHIFT_OPERAND);
	}

	if(ARM_IR_MSR_R) {
		if(pSPSR)
			arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), spsr(0, core));
	} else
		arm_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), CPSR);

	/* **** */

	if(pARMVM_TRACE)
		armvm_trace_msr(pARMVM_TRACE);
}

static void _arm_inst_umull(armvm_core_p const core)
{
	const uint32_t rs = arm_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));
	const uint32_t rm = arm_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));

	rSPR64(RESULT) = (uint32_t)rm * (uint32_t)rs;

	const uint32_t lo = rSPR64lo(RESULT);
	const uint32_t hi = rSPR64hi(RESULT);

	arm_reg_dst_wb(core, ARMVM_TRACE_R(DLo), ARM_IR_R(DLo), lo);
	arm_reg_dst_wb(core, ARMVM_TRACE_R(DHi), ARM_IR_R(DHi), hi);

	if(CCX && ARM_IR_DP_S) {
		ARM_CPSR_BMAS(N, BEXT(hi, 31));
		ARM_CPSR_BMAS(Z, (0 == rSPR64(RESULT)));
	}

	/* **** */

	if(pARMVM_TRACE)
		armvm_trace_umull(pARMVM_TRACE);
}

/* **** */

static void armvm_core_arm__step__group0_ldst(armvm_core_p const core)
{
	if(9 != mlBFTST(IR, 7, 4)) {
		if(ARM_IR_LDST_SH_BIT(I))
			return(_arm_inst_ldst_sh_immediate(core));
		else
			return(_arm_inst_ldst_sh_register(core));
	}

	return(__arm_decode_fail(core));
}

static void armvm_core_arm__step__group0_misc(armvm_core_p const core)
{
	switch(mlBFTST(IR, 27, 20) | mlBFTST(IR, 7, 4)) {
//		case 0x00000090:
//		case 0x00100090: return(_arm_inst_mul(core));
		case 0x00200090:
		case 0x00300090: return(_arm_inst_mla(core));
		case 0x00800090:
		case 0x00900090: return(_arm_inst_umull(core));
		case 0x01200030: return(_arm_inst_bx_blx_m(1, core));
		case 0x01200010: return(_arm_inst_bx_blx_m(0, core));
		case 0x01000000:
		case 0x01400000: return(_arm_inst_msr(core));
		case 0x01600010: return(_arm_inst_clz(core));
	}

	return(__arm_decode_fail(core));
}

static void armvm_core_arm__step_group0(armvm_core_p const core)
{
	if(BEXT(IR, 4)) {
		if(BEXT(IR, 7))
			return(armvm_core_arm__step__group0_ldst(core));
		else if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(armvm_core_arm__step__group0_misc(core));
		else
			return(_arm_inst_dp_shift_register(core));

	} else {
		if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(armvm_core_arm__step__group0_misc(core));
		else
			return(_arm_inst_dp_shift_immediate(core));
	}

	return(__arm_decode_fail(core));
}

static void armvm_core_arm__step__group1_misc(armvm_core_p const core)
{
	return(__arm_decode_fail(core));
}

static void armvm_core_arm__step_group1(armvm_core_p const core)
{
	if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
		return(armvm_core_arm__step__group1_misc(core));
	else
		return(_arm_inst_dp_immediate(core));

	return(__arm_decode_fail(core));
}

static void armvm_core_arm__step_group7(armvm_core_p const core)
{
	const uint32_t mask = mlBF(27, 24) | _BV(20) | _BV(4);

	switch(IR & mask) {
		case 0x0e000010:
			return(_arm_inst_mcr_mrc(core));
	}

	return(__arm_decode_fail(core));
}

void armvm_core_arm_step(armvm_core_p const core)
{
	IP = setup_rR_vR(core, ARMVM_TRACE_R(IP), ~0, PC); // STUPID KLUDGE!!
	PC = ARM_IP_NEXT;

	if(0 > armvm_core_mem_ifetch(&IR, IP, 4, core))
		return;

	setup_rR_vR(core, ARMVM_TRACE_R(IR), ~0, IR); // STUPID KLUDGE!!!

//	armvm_trace(pARMVM_TRACE);
//	arm_disasm(IP, IR);

	armvm_core_check_cc(ARM_IR_CC, core);

	switch(ARM_IR_CC) {
	default:
		switch(ARM_IR_GROUP) {
			case 0: // xxxx 000x xxxx xxxx
				return(armvm_core_arm__step_group0(core));
			case 1: // xxxx 001x xxxx xxxx
				return(armvm_core_arm__step_group1(core));
			case 2: // xxxx 010x xxxx xxxx
				return(_arm_inst_ldst_immediate(core));
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_b_bl(core));
			case 7: // xxxx 111x xxxx xxxx
				return(armvm_core_arm__step_group7(core));
			default:
				return(__arm_decode_fail(core));
		}
		break;
	case CC_NV:
		switch(ARM_IR_GROUP) {
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_blx(core));
			default:
				return(__arm_decode_fail(core));
		}
		break;
	}

	return(__arm_decode_fail(core));
}
