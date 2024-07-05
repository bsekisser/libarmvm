#include "armvm_core_cc.h"
#include "armvm_core_config.h"
#include "armvm_core_glue.h"
//#include "armvm_core_ldst.h"
#include "armvm_core_shiftbox.h"
#include "armvm_core_trace.h"

/* **** */

#include "armvm_trace.h"
#include "armvm.h"

/* **** */

#include "local/alubox.h"
#include "local/core_arm_trace.h"
#include "local/core_reg.h"
#include "local/ldst_arm.h"
#include "local/ldstm_arm.h"

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
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

/* **** */

static int __arm_decode_fail(armvm_core_p const core)
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
	LOG_ACTION(return(-1));
}

static int __arm__b_bl_blx(armvm_core_p const core, int const link, const int blx, int32_t const offset)
{
	const uint32_t new_pc  = ARM_PC_NEXT + offset;

	/* trace needs to occur before action as thumb state may change,
	 *	this will errantly be reflected in the trace
	 */

	_armvm_trace(core, "b%s%s(0x%08x)",
		link ? "l" : "",
		blx ? "x" : "", new_pc);

	/* **** */

	if(CCX) {
		if(link) {
			CYCLE++;
			LR = PC;
		}

		CYCLE++;
		PC = new_pc;

		if(blx) ARM_CPSR_BSET(Thumb);
	}

	return(0);
}

/* **** */

static void _arm_inst__ldstm(armvm_core_p const core,
	const unsigned user_mode_regs)
{
	if(ARM_IR_LDST_BIT(L)) {
		if((7 < rR(D)) && user_mode_regs)
			arm_ldm_user(core, rR(D), &vR(EA));
		else
			arm_ldm(core, rR(D), &vR(EA));
	} else {
		if((7 < rR(D)) && user_mode_regs)
			arm_stm_user(core, rR(D), &vR(EA));
		else
			arm_stm(core, rR(D), &vR(EA));
	}
}

static int _arm_inst_b_bl(armvm_core_p const core)
{
	return(__arm__b_bl_blx(core, ARM_IR_B_LINK, 0, ARM_IR_B_OFFSET));
}

static int _arm_inst_blx(armvm_core_p const core)
{
	CCX = 1; rSPR32(CC) = CC_AL;
	return(__arm__b_bl_blx(core, 1, 1, ARM_IR_BLX_OFFSET));
}

static int _arm_inst_bx_blx_m(armvm_core_p const core, const int link)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_RD);
		assert(15 == ARM_IR_RN);
		assert(15 == ARM_IR_RS);
	}

	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const unsigned thumb = CONFIG->features.thumb ? (rm & 1) : 0;

	/* trace needs to occur before action as thumb state may change,
	 *	this will errantly be reflected in the trace
	 */

	if(_armvm_trace_start(core, "b%sx(%s)",
		link ? "l" : "", irR_NAME(M))) {

		_armvm_trace_comment(core, "%s(0x%08x)", thumb ? "T" : "A", rm);

		__trace_end(core);
	}

	/* **** */

	if(CCX) {
		if(link) {
			CYCLE++;
			LR = PC;
		}

		CYCLE++;
		armvm_core_pcx(core, rm);
	}

	return(0);
}

static int _arm_inst_clz(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(15 == ARM_IR_R(S));
	}

	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	uint32_t rd = rn ? 31 : 0;

	if(rn) {
//		while(rd && rn) { rd--; rn >>= 1; }
		while(rd && !BEXT(rn, rd)) rd--;
	}

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);

	if(core->config.trace) // TODO
		LOG_ACTION(exit(-1));

	return(rR_IS_PC(D));
}

static int _arm_inst_dp(armvm_core_p const core)
{
	core_reg_src_setup(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	core_reg_dst(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	alubox(core, ARM_IR_DP_OPCODE, ARM_IR_DP_S, 1);

	if(core->config.trace)
		armvm_trace_dp(core);

	return(rR_IS_NOT_PC(D));
}

static int _arm_inst_dp_immediate(armvm_core_p const core)
{
	const uint32_t rm = setup_vR(core, ARMVM_TRACE_R(M), ARM_IR_DPI_IMMEDIATE);
	const uint32_t rs = setup_vR(core, ARMVM_TRACE_R(S), ARM_IR_DPI_ROTATE_AMOUNT);

	const uint32_t sop = arm_shiftbox(ARM_SOP_ROR, rm, rs, IF_CPSR(C));

	(void)setup_vR(core, ARMVM_TRACE_R(SOP), sop);

	return(_arm_inst_dp(core));
}

static int _arm_inst_dp_shift(armvm_core_p const core, unsigned immediate)
{
	if(CCX) CYCLE++;

	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));

	const uint32_t sop = (immediate ? arm_shiftbox_immediate : arm_shiftbox)
		(ARM_IR_DP_SHIFT_TYPE, rm, vR(S), IF_CPSR(C));

	setup_vR(core, ARMVM_TRACE_R(SOP), sop);

	return(_arm_inst_dp(core));
}

static int _arm_inst_dp_shift_register(armvm_core_p const core)
{
	if(CCX) CYCLE++;

	(void)core_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));

	return(_arm_inst_dp_shift(core, 0));
}

static int _arm_inst_dp_shift_immediate(armvm_core_p const core)
{
	(void)setup_vR(core, ARMVM_TRACE_R(S), ARM_IR_DP_SHIFT_AMOUNT);

	return(_arm_inst_dp_shift(core, 1));
}

static int _arm_inst_ldst(armvm_core_p const core)
{
	setup_rR(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	int (*ldst_fn)(armvm_core_p const core);
	if(ARM_IR_LDST_BIT(L)) {
		ldst_fn = ARM_IR_LDST_BIT(B) ? __ldrb : __ldr_arm;
	} else {
		ldst_fn = ARM_IR_LDST_BIT(B) ? __strb : __str;
	}

	const int ldst_rval = __ldst_arm_ea_fn(core, ldst_fn);

	if(core->config.trace)
		armvm_trace_ldst(core);

	return(ldst_rval);
}

static int _arm_inst_ldst_immediate(armvm_core_p const core)
{
	const uint32_t rm = setup_vR(core, ARMVM_TRACE_R(M), ARM_IR_LDST_IMMEDIATE_OFFSET);
	(void)setup_vR(core, ARMVM_TRACE_R(SOP), rm);

	return(_arm_inst_ldst(core));
}

static int _arm_inst_ldst_scaled_register_offset(armvm_core_p const core)
{
	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const uint32_t rs = setup_vRml(core, ARMVM_TRACE_R(S), 11, 7);

	const uint32_t sop = arm_shiftbox_immediate(ARM_IR_DP_SHIFT_TYPE, rm, rs, IF_CPSR(C));
	(void)setup_vR(core, ARMVM_TRACE_R(SOP), sop);

	return(_arm_inst_ldst(core));
}

static int _arm_inst_ldst_sh(armvm_core_p const core, const uint32_t rm)
{
	(void)setup_vR(core, ARMVM_TRACE_R(SOP), rm);
	setup_rR(core, ARMVM_TRACE_R(D), ARM_IR_R(D));

	const unsigned bwh  = BMOV(IR, ARM_IR_LDST_BIT_L, 2) | mlBFEXT(IR, 6, 5);

	int (*ldst_fn)(armvm_core_p const core);
	switch(bwh) {
		case 1: ldst_fn = __strh; break;
//		case 2: ldst_fn = __ldrd; break;
//		case 3: ldst_fn = __strd; break;
		case 5: ldst_fn = __ldrh; break;
		case 6: ldst_fn = __ldrsb; break;
		case 7: ldst_fn = __ldrsh; break;
		default:
			LOG_ACTION(return(__arm_decode_fail(core)));
	}

	const int ldst_rval = __ldst_arm_ea_fn(core, ldst_fn);

	if(core->config.trace)
		armvm_trace_ldst(core);

	return(ldst_rval);
}

static int _arm_inst_ldst_sh_immediate(armvm_core_p const core)
{
	const uint rm = setup_vR(core, ARMVM_TRACE_R(M), ARM_IR_LDST_SH_OFFSET);
	return(_arm_inst_ldst_sh(core, rm));
}

static int _arm_inst_ldst_sh_register(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks)
		assert(0 == ARM_IR_RS);

	const uint rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	return(_arm_inst_ldst_sh(core, rm));
}

static int _arm_inst_ldstm(armvm_core_p const core)
{
	const uint32_t rm = setup_vRml(core, ARMVM_TRACE_R(M), 15, 0);
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));

	const uint8_t _rcount = (uint8_t)__builtin_popcount(rm);
	const uint8_t rcount_bytes = _rcount << 2;

	const uint32_t sp_in = rn;

	/*
	 * DA	!LDST_BIT(u23) && !LDST_BIT(p24)
	 * DB	!LDST_BIT(u23) && LDST_BIT(p24)
	 * IA	LDST_BIT(u23) && !LDST_BIT(p24)
	 * IB	LDST_BIT(u23) && LDST_BIT(p24)
	 *
	 */

	const int bit_l = ARM_IR_LDST_BIT(L);
	const int bit_p = ARM_IR_LDST_BIT(P);
	const int bit_u = ARM_IR_LDST_BIT(U);

	const uint32_t pre_offset = bit_p << 2;
	const uint32_t post_offset = !bit_p << 2;

	const uint32_t rcount_offset = bit_u ? rcount_bytes : -rcount_bytes;

	const uint32_t start_offset = (bit_u ? pre_offset : (rcount_offset + post_offset));
	const uint32_t end_offset = (bit_u ? (rcount_offset - post_offset) : -pre_offset);

	const uint32_t start_address = rn + start_offset;
	const uint32_t end_address = rn + end_offset;

	const uint32_t sp_out = sp_in + rcount_offset;

	if(0) LOG("sp_in = 0x%08x, start_address = 0x%08x, end_address = 0x%08x",
		sp_in, start_address, end_address);
	if(0) LOG("sp_out = 0x%08x", sp_out);

	const char *opstr; (void)opstr;
	if(0 && (ARMVM_GPR(SP) == rR(N)))
		opstr = bit_l ? "pop" : "push";
	else
		opstr = bit_l ? "ldm" : "stm";

	char reglist[17]; (void)reglist;
	for(int i = 0; i <= 15; i++)
	{
		uint8_t c = (i > 9 ? ('a' + (i - 10)) : '0' + i);
		reglist[i] = BTST(vR(M), i) ? c : '.';
	}
	reglist[16] = 0;

	const int bit_s = ARM_IR_LDSTM_BIT(S);
	const int bit_w = ARM_IR_LDST_BIT(W);

	const int flag_pc = BTST(rm, 15);
	const int flag_sl = bit_s && bit_l;

	const int load_spsr = flag_sl && flag_pc;

	const int user_mode_regs_load = flag_sl && !bit_w && !flag_pc;
	const int user_mode_regs_store = bit_s && !bit_l;

	if(0) LOG("s = %01u, umrl = %01u, umrs = %01u", bit_s, user_mode_regs_load, user_mode_regs_store);

	const int user_mode_regs = user_mode_regs_load || user_mode_regs_store;

	if(__trace_start(core)) {
		_armvm_trace_(core, "%s%c%c(%s%s, {%s}%s%s)",
			opstr, bit_u ? 'i' : 'd', bit_p ? 'b' : 'a',
			irR_NAME(N), bit_w ? "!" : "", reglist,
			user_mode_regs ? ", USER" : "",
			load_spsr ? ", SPSR" : "");

		_armvm_trace_comment(core, "0x%08x", sp_in);

		__trace_end(core);
	}

	setup_vR(core, ARMVM_TRACE_R(EA), start_address);

	if(CCX)
	{
		if(CP15_REG1_BIT(U)) {
			if(vR(EA) & 3)
				return(armvm_core_exception_data_abort(core));
		}
		else
			vR(EA) &= ~3U;

		for(rR(D) = 0; rR(D) < 15; rR(D)++)
		{
			if(BTST(vR(M), rR(D)))
			{
				CYCLE++;
				_arm_inst__ldstm(core, user_mode_regs);
			}
		}

		if(BTST(vR(M), 15)) {
			if(bit_l)
				arm_ldm_pc(core, &vR(EA));
			else
				arm_stm(core, ARMVM_GPR(PC), &vR(EA));
		}

		if(load_spsr && pSPSR) {
			LOG_ACTION(armvm_core_psr_mode_switch(core, armvm_core_spsr(core, 0)));
		}

		if((bit_w && (user_mode_regs || load_spsr))
			|| (user_mode_regs && load_spsr))
				LOG_ACTION(exit(-1));

		if(bit_w)
		{
			if(0) LOG("ea = 0x%08x", vR(EA));

			assert(end_address == vR(EA) - 4);
			if(end_address == vR(EA) - 4)
				core_reg_wb_v(core, ARMVM_TRACE_R(N), sp_out);
			else
					LOG_ACTION(exit(-1));
//				UNDEFINED;
		}
	}

	return(!flag_pc);
}

static int _arm_inst_mcr_mrc(armvm_core_p const core)
{
	if(core->config.trace) {
		_armvm_trace(core, "m%s(p(%u), %u, %s, %s, %s, %u)",
			ARM_IR_MCRC_L ? "rc" : "cr", ARM_IR_MCRC_CPx, ARM_IR_MCRC_OP1,
			irR_NAME(D),
			arm_creg_name_string[ARM_IR_MCRC_CRn], arm_creg_name_string[ARM_IR_MCRC_CRm],
			ARM_IR_MCRC_OP2);
	}

	if(!CCX)
		return(1);

	if(ARM_IR_MCRC_L) {
		uint32_t rd = armvm_coprocessor_access(core->cp, 0);

		if(rR_IS_PC(D)) {
			ARM_CPSR_BIC(ARM_CPSR_MASK_NZCV, rd);
		} else
			irGPR(D) = rd;
	} else
		armvm_coprocessor_access(core->cp, &irGPR(D));

	return(rR_IS_NOT_PC(D));
}

static int _arm_inst_mla(armvm_core_p const core)
{
	const uint32_t rn = core_reg_src(core, ARMVM_TRACE_R(N), ARM_IR_R(N));
	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const uint32_t rs = core_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));

	const uint32_t rd = (rm * rs) + rn;

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);

	if(CCX && ARM_IR_DP_S)
		_alubox_flags_nz(core, irGPR(D));

	/* **** */

	if(__trace_start(core)) {
		_armvm_trace_(core, "mla%s(", ARM_IR_DP_S ? "s" : "");
		_armvm_trace_(core, "%s", irR_NAME(D));
		_armvm_trace_(core, ", %s", irR_NAME(M));
		_armvm_trace_(core, ", %s", irR_NAME(S));
		_armvm_trace_(core, ", %s)", irR_NAME(N));

		_armvm_trace_comment(core, "(0x%08x * 0x%08x) + 0x%08x = 0x%08x",
			rm, rs, rn, rd);

		__trace_end(core);
	}

	return(rR_IS_NOT_PC(D));
}

static int _arm_inst_mrs(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(N));
		assert(0 == ARM_IR_SHIFT_OPERAND);
	}

	uint32_t rd = 0;

	if(ARM_IR_MRSR_R) {
		if(pSPSR)
			rd = core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), armvm_core_spsr(core, 0));
	} else
		rd = core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), CPSR);

	/* **** */

	if(_armvm_trace_start(core, "mrs(%s, %s)", irR_NAME(D), ARM_IR_MRSR_R ? "spsr" : "cpsr")) {
		_armvm_trace_comment(core, "0x%08x", rd);
		__trace_end(core);
	}

	return(rR_IS_NOT_PC(D));
}

static const uint32_t _armvm_core_msr_priv_mask[] =
	{ 0x0000000f, 0x0000000f, 0x0000000f, 0x0000000f, 0x000001df };
static const uint32_t _armvm_core_msr_state_mask[] =
	{ 0x00000000, 0x00000020, 0x00000020, 0x01000020, 0x01000020 };
static const uint32_t _armvm_core_msr_unalloc_mask[] =
	{ 0x0fffff20, 0x0fffff00, 0x07ffff00, 0x06ffff00, 0x06f0fc00 };
static const uint32_t _armvm_core_msr_user_mask[] =
	{ 0xf0000000, 0xf0000000, 0xf8000000, 0xf8000000, 0xf80f0200 };

static int _arm_inst_msr(armvm_core_p const core, const uint32_t sop)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(15 == ARM_IR_R(D));
	}

	const unsigned arm_version = core->config.version;
if(0) LOG("arm_version: 0x%08x", arm_version);

	const uint32_t unalloc_mask = _armvm_core_msr_unalloc_mask[arm_version];
if(0) LOG("unalloc_mask: 0x%08x", unalloc_mask);

	if(sop & unalloc_mask)
	{
		/* TODO: UNPREDICTABLE
		 *
		 * till further notice, fail silently...  fuck it!
		 */
	}

	const uint32_t field_mask = ARM_IR_R(N);
if(0) LOG("field_mask: 0x%08x", field_mask);

	const uint32_t byte_mask =
		(BTST(field_mask, 0) ? (0xff << (0 << 3)) : 0)
		| (BTST(field_mask, 1) ? (0xff << (1 << 3)) : 0)
		| (BTST(field_mask, 2) ? (0xff << (2 << 3)) : 0)
		| (BTST(field_mask, 3) ? (0xff << (3 << 3)) : 0);
if(0) LOG("byte_mask: 0x%08x", byte_mask);

	const uint32_t state_mask = _armvm_core_msr_state_mask[arm_version];
if(0) LOG("state_mask: 0x%08x", state_mask);

	const uint32_t user_mask = _armvm_core_msr_user_mask[arm_version];
if(0) LOG("user_mask: 0x%08x", user_mask);

	const uint32_t priv_mask = _armvm_core_msr_priv_mask[arm_version];
if(0) LOG("priv_mask: 0x%08x", priv_mask);

	uint32_t saved_psr = 0, new_psr = 0;

	uint32_t mask = 0;
	if(ARM_IR_MRSR_R)
	{
		if(pSPSR)
		{
			mask = byte_mask & (user_mask | priv_mask | state_mask);

			saved_psr = armvm_core_spsr(core, 0);
			new_psr = (saved_psr & ~mask) | (sop & mask);

			if(CCX)
				armvm_core_spsr(core, &new_psr);
		}
		else
		{
			LOG_ACTION(UNPREDICTABLE);
		}
	}
	else
	{
		if(armvm_core_in_a_privaleged_mode(core))
		{
			if(sop & state_mask)
			{
				LOG_ACTION(UNPREDICTABLE);
			}
			else
				mask = byte_mask & (user_mask | priv_mask);
		}
		else
			mask = byte_mask & user_mask;

if(0)	LOG("mask: 0x%08x", mask);

		saved_psr = CPSR;
		new_psr = (saved_psr & ~mask) | (sop & mask);

		if(CCX)
			armvm_core_psr_mode_switch_cpsr(core, new_psr);
	}

	if(core->config.trace) {
		setup_vR(core, ARMVM_TRACE_R(D), new_psr);
		setup_vR(core, ARMVM_TRACE_R(N), saved_psr);
		setup_rR_vR(core, ARMVM_TRACE_R(S), ~field_mask, mask);

		armvm_trace_msr(core);
	}

	return(0);
}

static int _arm_inst_msr_register(armvm_core_p const core)
{
	if(CONFIG->pedantic.ir_checks) {
		assert(0 == ARM_IR_R(S));
	}

	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const uint32_t sop = setup_vR(core, ARMVM_TRACE_R(SOP), rm);

	return(_arm_inst_msr(core, sop));
}

static int _arm_inst_mul(armvm_core_p const core)
{
	const uint32_t rs = core_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));
	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));

	rSPR64(RESULT) = rm * rs;
	const uint32_t rd = rSPR64lo(RESULT);

	core_reg_dst_wb(core, ARMVM_TRACE_R(D), ARM_IR_R(D), rd);
	if(CCX) _alubox_flags_nz(core, rd);

	/* **** */

	if(__trace_start(core)) {
		_armvm_trace_(core, "mul%s(%s, %s, %s)",
			ARM_IR_DP_S ? "s" : "", irR_NAME(D), irR_NAME(M), irR_NAME(S));

		_armvm_trace_comment(core, "0x%08x * 0x%08x = 0x%08x",
			rm, rs, rd);

		__trace_end(core);
	}

	return(rR_IS_NOT_PC(D));
}

static int _arm_inst_smull(armvm_core_p const core)
{
	const int32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));
	const int32_t rs = core_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));

	rSPR64(RESULT) = (int64_t)(rm * rs);

	const uint32_t lo = rSPR64lo(RESULT);
	const uint32_t hi = rSPR64hi(RESULT);

	core_reg_dst_wb(core, ARMVM_TRACE_R(DLo), ARM_IR_R(DLo), lo);
	core_reg_dst_wb(core, ARMVM_TRACE_R(DHi), ARM_IR_R(DHi), hi);

	if(CCX && ARM_IR_DP_S) {
		ARM_CPSR_BMAS(N, BEXT(hi, 31));
		ARM_CPSR_BMAS(Z, (0 == rSPR64(RESULT)));
	}

	/* **** */

	if(__trace_start(core)) {
		_armvm_trace_(core, "smull%s(%s:%s, %s, %s)",
			ARM_IR_DP_S ? "s" : "",
			irR_NAME(DLo), irR_NAME(DHi), irR_NAME(M), irR_NAME(S));

		_armvm_trace_comment(core, "0x%08x * 0x%08x = 0x%016" PRIx64,
			rm, rs, rSPR64(RESULT));

		__trace_end(core);
	}

	return(rR_IS_NOT_PC(D));
}

static int _arm_inst_umull(armvm_core_p const core)
{
	const uint32_t rs = core_reg_src(core, ARMVM_TRACE_R(S), ARM_IR_R(S));
	const uint32_t rm = core_reg_src(core, ARMVM_TRACE_R(M), ARM_IR_R(M));

	rSPR64(RESULT) = (uint32_t)rm * (uint32_t)rs;

	const uint32_t lo = rSPR64lo(RESULT);
	const uint32_t hi = rSPR64hi(RESULT);

	core_reg_dst_wb(core, ARMVM_TRACE_R(DLo), ARM_IR_R(DLo), lo);
	core_reg_dst_wb(core, ARMVM_TRACE_R(DHi), ARM_IR_R(DHi), hi);

	if(CCX && ARM_IR_DP_S) {
		ARM_CPSR_BMAS(N, BEXT(hi, 31));
		ARM_CPSR_BMAS(Z, (0 == rSPR64(RESULT)));
	}

	/* **** */

	if(_armvm_trace_start(core, "umull%s(", ARM_IR_DP_S ? "s" : "")) {
		_armvm_trace_(core, "%s", irR_NAME(DLo));
		_armvm_trace_(core, ":%s", irR_NAME(DHi));
		_armvm_trace_(core, ", %s", irR_NAME(M));
		_armvm_trace_(core, ", %s)", irR_NAME(S));

		_armvm_trace_comment(core, "0x%08x * 0x%08x = 0x%08x%08x",
			vR(M), vR(S), vR(DHi), vR(DLo));

		__trace_end(core);
	}

	return(rR_IS_NOT_PC(D));
}

/* **** */

static int armvm_core_arm__step__group0_ldst(armvm_core_p const core)
{
	if(9 != mlBFEXT(IR, 7, 4)) {
		if(ARM_IR_LDST_SH_BIT(I))
			return(_arm_inst_ldst_sh_immediate(core));
		else
			return(_arm_inst_ldst_sh_register(core));
	} else {
		switch(mlBFTST(IR, 27, 20) | mlBFTST(IR, 7, 4)) {
			case 0x00000090:
			case 0x00100090:
				return(_arm_inst_mul(core));
			case 0x00200090:
			case 0x00300090:
				return(_arm_inst_mla(core));
			case 0x00800090:
			case 0x00900090:
				return(_arm_inst_umull(core));
			case 0x00c00090:
			case 0x00d00090:
				return(_arm_inst_smull(core));
		}
	}

	LOG_ACTION(return(__arm_decode_fail(core)));
}

static int armvm_core_arm__step__group0_misc(armvm_core_p const core)
{
	switch(mlBFTST(IR, 27, 20)) {
		case 0x01000000:
		case 0x01400000: return(_arm_inst_mrs(core));
	}

	switch(mlBFTST(IR, 27, 20) | mlBFTST(IR, 7, 4)) {
		case 0x01200000:
		case 0x01600000: return(_arm_inst_msr_register(core));
		case 0x01200010: return(_arm_inst_bx_blx_m(core, 0));
		case 0x01200030: return(_arm_inst_bx_blx_m(core, 1));
		case 0x01600010: return(_arm_inst_clz(core));
	}

	LOG_ACTION(return(__arm_decode_fail(core)));
}

static int armvm_core_arm__step_group0(armvm_core_p const core)
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

	LOG_ACTION(return(__arm_decode_fail(core)));
}

static int armvm_core_arm__step__group1_misc(armvm_core_p const core)
{
	LOG_ACTION(return(__arm_decode_fail(core)));
}

static int armvm_core_arm__step_group1(armvm_core_p const core)
{
	if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
		return(armvm_core_arm__step__group1_misc(core));
	else
		return(_arm_inst_dp_immediate(core));

	LOG_ACTION(return(__arm_decode_fail(core)));
}

static int armvm_core_arm__step_group7(armvm_core_p const core)
{
	const uint32_t mask = mlBF(27, 24) | _BV(20) | _BV(4);

	switch(IR & mask) {
		case 0x0e000010: // mcr
		case 0x0e100010: // mrc
			return(_arm_inst_mcr_mrc(core));
	}

	LOG_ACTION(return(__arm_decode_fail(core)));
}

int armvm_core_arm_step(armvm_core_p const core)
{
	IP = setup_vR(core, ARMVM_TRACE_R(IP), PC & ~3U); // STUPID KLUDGE!!
	PC = ARM_IP_NEXT;

	if(0 > armvm_core_mem_ifetch(core, &IR, IP, 4))
		return(1);

	setup_vR(core, ARMVM_TRACE_R(IR), IR); // STUPID KLUDGE!!!

//	armvm_trace(pARMVM_TRACE);
//	arm_disasm(IP, IR);

	armvm_core_check_cc(core, ARM_IR_CC);

	switch(ARM_IR_CC) {
	default:
		switch(ARM_IR_GROUP) {
			case 0: // xxxx 000x xxxx xxxx
				return(armvm_core_arm__step_group0(core));
			case 1: // xxxx 001x xxxx xxxx
				return(armvm_core_arm__step_group1(core));
			case 2: // xxxx 010x xxxx xxxx
				return(_arm_inst_ldst_immediate(core));
			case 3: // xxxx 011x xxxx xxxx
				if(!BTST(IR, 4))
					return(_arm_inst_ldst_scaled_register_offset(core));
				else
					LOG_ACTION(return(__arm_decode_fail(core)));
				break;
			case 4: /* xxxx 100x xxxx xxxx */
				return(_arm_inst_ldstm(core));
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_b_bl(core));
			case 7: // xxxx 111x xxxx xxxx
				return(armvm_core_arm__step_group7(core));
			default:
				LOG_ACTION(return(__arm_decode_fail(core)));
		}
		break;
	case CC_NV:
		switch(ARM_IR_GROUP) {
			case 5: // xxxx 101x xxxx xxxx
				return(_arm_inst_blx(core));
			default:
				LOG_ACTION(return(__arm_decode_fail(core)));
		}
		break;
	}

	LOG_ACTION(return(__arm_decode_fail(core)));
}
