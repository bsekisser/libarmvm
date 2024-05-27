#include "armcc.hpp"

/* **** */

extern "C" {
	#include "armcc_gen_arm.h"
	#include "armcc_gen_arm_dp.h"
	#include "armcc_gen_arm_dp_sop.h"
	#include "armcc_gen_arm_ir.h"
	#include "armcc_gen.h"
}

/* **** */

extern "C" {
	#include "libarm/include/arm_cc.h"
	#include "libarm/include/arm_dp.h"
	#include "libarm/include/arm_ir.h"
	#include "libarm/include/arm_sop.h"
}

extern "C" {
	#include "libbse/include/bitfield.h"
	#include "libbse/include/err_test.h"
	#include "libbse/include/handle.h"
	#include "libbse/include/log.h"
}

/* **** */

extern "C" {
	#include <assert.h>
	#include <endian.h>
	#include <stdint.h>
}

/* **** */

uint32_t armcc::add(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm)
{
	return(gen_arm_dp__op_s_rd_rn(&cc, 1, ARM_ADD, 0, rd, rn, gen_arm_dp_sop__ror_i(imm, 0)));
}

uint32_t armcc::ands(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	return(gen_arm_dp__op_s_rd_rn_rm(&cc, ARM_AND, 1, rd, rn, rm));
}

armcc::armcc(void *const p2data):armcc(p2data, 0, 0) {}

armcc::armcc(void *const p2data, armcc_h h2armcc_t):armcc(p2data)
{ *h2armcc_t = p2armcc_t(); }

armcc::armcc(void *const p2data, const uint32_t cs, const uint32_t ds)
{
	if(0) {
		LOG_START("p2data: 0x%016" PRIxPTR, (uintptr_t)p2data);
		_LOG_(", cs: 0x%08x", cs);
		LOG_END(", ds: 0x%08x", ds);
	}

	ERR_NULL(p2data);
	cc.p2data = p2data;

	org_data(ds);
	org_text(cs);
}

armcc_sop_t armcc::asr(const arm_reg_t rm, const uint8_t rs)
{ return(gen_arm_dp_sop__rm_i(rm, ARM_SOP_ASR, rs)); }

uint32_t armcc::b(const uint32_t pat)
{ return(gen_arm__group_ir(&cc, 5, gen_arm__b_offset(&cc, pat))); }

uint32_t armcc::bne(const uint32_t pat)
{ return(gen_arm__cc_group_ir(&cc, CC_NE, 5, gen_arm__b_offset(&cc, pat))); }

uint32_t armcc::cmp(const arm_reg_t rn, const arm_reg_t rm)
{ return(gen_arm_dp__op_s_rd_rn_rm(&cc, ARM_CMP, 1, r0, rn, rm)); }

uint32_t armcc::dw(const uint32_t data)
{
	const uint32_t dp = cc.dp;
	const uint32_t ds_dp = cc.ds + dp;

	cc.dp += 4;

	if(0) {
		LOG_START("(ds: 0x%08x, dp: 0x%08x): 0x%08x", cc.ds, dp, ds_dp);
		LOG_END(", data: 0x%08x", data);
	}

	uint8_t* p2ds = (uint8_t*)cc.p2ds;
	uint32_t* p = (uint32_t*)(p2ds + (dp & 0xffff));
	*p = htole32(data);
	cc.dcount++;

	return(ds_dp);
}

uint32_t armcc::ldr(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat)
{
	unsigned bit_u = 0;
	const uint32_t offset = gen_arm__ldst_offset(&cc, pat, &bit_u);

	const uint32_t ir = (1 << ARM_IR_LDST_BIT_L)
		| (1 << ARM_IR_LDST_BIT_P)
		| (bit_u << ARM_IR_LDST_BIT_U);

	return(gen_arm__group_rd_rn_x(&cc, 2, rd, rn, ir + offset));
}

armcc_sop_t armcc::lsl(const arm_reg_t rm, const uint8_t rs)
{ return(gen_arm_dp_sop__rm_i(rm, ARM_SOP_LSL, rs)); }

armcc_sop_t armcc::lsr(const arm_reg_t rm, const uint8_t rs)
{ return(gen_arm_dp_sop__rm_i(rm, ARM_SOP_LSR, rs)); }

uint32_t armcc::mov(const arm_reg_t rd, const arm_reg_t rm)
{ return(gen_arm_dp__op_s_rd_rn_rm(&cc, ARM_MOV, 0, rd, r0, rm)); }

uint32_t armcc::mov(const arm_reg_t rd, const uint8_t imm)
{
	return(gen_arm_dp__op_s_rd_rn(&cc, 1, ARM_MOV, 0, rd, r0, gen_arm_dp_sop__ror_i(imm, 0)));
}

uint32_t armcc::org_data(const uint32_t ds)
{
	cc.ds = ds;

	cc.p2ds = ((uint8_t*)cc.p2data) + ds;
	cc.dp = 0;

	cc.dcount = 0;

	if(0) {
		LOG("p2data: 0x%016" PRIxPTR ", p2ds: 0x%016" PRIxPTR,
			(uintptr_t)cc.p2data, (uintptr_t)cc.p2ds);
	}

	return(cc.ds + cc.dp);
}

uint32_t armcc::org_text(const uint32_t cs)
{
	cc.cs = cs;

	cc.p2cs = ((uint8_t*)cc.p2data) + cs;
	cc.pc = 0;

	cc.icount = 0;

	const uint32_t cs_ip = cc.cs + cc.ip;
	const uint32_t cs_pc = cc.cs + cc.pc;

	if(0) {
		LOG_START("(cs: 0x%08x, ip: 0x%08x): 0x%08x", cc.cs, cc.ip, cs_ip);
		_LOG_(", pc: 0x%08x", cc.pc);
		LOG_END(", (cs + pc): 0x%08x", cs_pc);

		LOG("p2data: 0x%016" PRIxPTR ", p2cs: 0x%016" PRIxPTR,
			(uintptr_t)cc.p2data, (uintptr_t)cc.p2cs);
	}

	return(cs_pc);
}

armcc_p armcc::p2armcc_t(void)
{ return(&cc); }

armcc_sop_t armcc::ror(const arm_reg_t rm, const uint8_t rs)
{ return(gen_arm_dp_sop__rm_i(rm, ARM_SOP_ROR, rs)); }

uint32_t armcc::sbcs(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm)
{
	return(gen_arm_dp__op_s_rd_rn(&cc, 1, ARM_SBC, 1, rd, rn, gen_arm_dp_sop__ror_i(imm, 0)));
}

uint32_t armcc::sbcs(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	return(gen_arm_dp__op_s_rd_rn_rm(&cc, ARM_SBC, 1, rd, rn, rm));
}

uint32_t armcc::str(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat)
{
	unsigned bit_u = 0;
	const uint32_t offset = gen_arm__ldst_offset(&cc, pat, &bit_u);

	const uint32_t ir = (bit_u << ARM_IR_LDST_BIT_U)
		| (1 << ARM_IR_LDST_BIT_P);

	return(gen_arm__group_rd_rn_x(&cc, 2, rd, rn, ir + offset));
}

uint32_t armcc::subs(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm)
{
	return(gen_arm_dp__op_s_rd_rn(&cc, 1, ARM_SUB, 1, rd, rn, gen_arm_dp_sop__ror_i(imm, 0)));
}

uint32_t armcc::subs(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	return(gen_arm_dp__op_s_rd_rn_rm(&cc, ARM_SUB, 1, rd, rn, rm));
}

uint32_t armcc::subs(const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop)
{
	return(gen_arm_dp__op_s_rd_rn(&cc, 0, ARM_SUB, 1, rd, rn, sop));
}
