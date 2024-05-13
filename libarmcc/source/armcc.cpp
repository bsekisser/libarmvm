#include "armcc.hpp"

/* **** */

extern "C" {
	#include "libarm/include/arm_cc.h"
	#include "libarm/include/arm_dp.h"
	#include "libarm/include/arm_ir.h"
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
	#include <stdint.h>
}

/* **** */

typedef struct armcc_t* armcc_p;

static uint32_t __gen_arm_ir_b_offset(armcc_p p2cc, const uint32_t pat)
{
	const uint32_t cs_pc = p2cc->cs + p2cc->pc;

	if(0) LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);

	const uint32_t offset = pbBFMOVs((-8 + (pat - cs_pc)), 2, 24, 0);
	const uint32_t offset_masked = offset & pbBF(0, 24);

	if(0) {
		LOG_START("offset: 0x%08x", offset);
		LOG_END(", offset_masked: 0x%08x", offset_masked);
	}

	assert(offset == pbBFEXTs(offset_masked, 0, 24));

	return(offset_masked);
}

static uint32_t __gen_arm_ir_cc(const arm_condition_t cc)
{
	assert(0 == (cc & ~15));
	return(pbBFMOV(cc, 0, 4, 28));
}

static uint32_t __gen_arm_ir_dp_opcode(const arm_dp_opcode_t opcode, const unsigned s)
{
	assert(0 == (opcode & ~15));
	return(pbBFMOV(opcode, 0, 4, 21) | (s << 20));
}

static uint32_t __gen_arm_ir_group(const unsigned group)
{
	assert(0 == (group & ~7));
	return(pbBFMOV(group, 0, 3, 25));
}

static uint32_t __gen_arm_ir_ldst_offset(armcc_p p2cc,
	const unsigned pat, unsigned *const bit_u)
{
	assert(p2cc);
	assert(bit_u);

	const uint32_t cs_pc = p2cc->cs + p2cc->pc;

	if(0) {
		LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);
	}

	const uint32_t offset = -8 + (pat - cs_pc);
	const uint32_t offset_masked = offset & pbBF(0, 12);

	if(0) {
		LOG_START("offset: 0x%08x", offset);
		LOG_END(", offset_masked: 0x%08x", offset_masked);
	}

//	assert(offset == pbBFEXTs(offset_masked, 0, 12));

	*bit_u = !((int32_t)offset < 0);

	return(offset_masked);
}

static uint32_t __gen_arm_ir_rd(const arm_reg_t rd)
{
	assert(0 == (rd & ~15));
	return(pbBFMOV(rd, 0, 4, 12));
}

static uint32_t __gen_arm_ir_rn(const arm_reg_t rn)
{
	assert(0 == (rn & ~15));
	return(pbBFMOV(rn, 0, 4, 16));
}

static uint32_t __gen_arm_ir_rm(const arm_reg_t rm)
{
	assert(0 == (rm & ~15));
	return(rm & 15);
}

static uint32_t __gen_rd_rn(const arm_reg_t rd, const arm_reg_t rn)
{ return(__gen_arm_ir_rd(rd) | __gen_arm_ir_rn(rn)); }

static uint32_t __gen_rd_rn_rm(const arm_reg_t rd, const arm_reg_t rn,
	const arm_reg_t rm)
{ return(__gen_rd_rn(rd, rn) | __gen_arm_ir_rm(rm)); }

static arm_sop_t __gen_arm_dp_sop_ror_i(const uint8_t imm, const uint8_t rs)
{ return((imm & 0xff) | pbBFMOV(rs, 1, 4, 8)); }

/* **** */

static uint32_t __gen(armcc_p const p2cc, const arm_condition_t cc,
	const unsigned group, const uint32_t ir_in)
{
	p2cc->ip = p2cc->pc;
	p2cc->pc += 4;

	const uint32_t cs_ip = p2cc->cs + p2cc->ip;
	const uint32_t cs_pc = p2cc->cs + p2cc->pc;

	const uint32_t ir = __gen_arm_ir_cc(cc) | __gen_arm_ir_group(group) | ir_in;

	if(0) {
		LOG_START("(cs: 0x%08x, ip: 0x%08x): 0x%08x", p2cc->cs, p2cc->ip, cs_ip);
		_LOG_(", pc: 0x%08x", p2cc->pc);
		_LOG_(", (cs + pc): 0x%08x", cs_pc);
		LOG_END(", ir: 0x%08x", ir);
	}

	uint8_t* p2cs = (uint8_t*)p2cc->p2cs;
	uint32_t* p = (uint32_t*)(p2cs + (p2cc->ip & 0xffff));

	if(0) {
		LOG_START("p2cs: 0x%016" PRIxPTR, (uintptr_t)p2cs);
		LOG_END(", p: 0x%016" PRIxPTR, (uintptr_t)p);
	}

	*p = ir;

	return(cs_pc);
}

static uint32_t _gen(armcc_p const p2cc, const unsigned group, const uint32_t ir)
{ return(__gen(p2cc, CC_AL, group, ir)); }

static uint32_t _gen_cc(armcc_p const p2cc, const arm_condition_t cc,
	const unsigned group, const uint32_t ir)
{ return(__gen(p2cc, cc, group, ir)); }

static uint32_t _gen_dp_op_s_rd_rn(armcc_p const p2cc, const unsigned group,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const arm_sop_t sop)
{
	return(_gen(p2cc, group,
		__gen_arm_ir_dp_opcode(opcode, s)
			| __gen_rd_rn(rd, rn)
			| sop)
		);
}

static uint32_t _gen_dp_op_s_rd_rn_rm(armcc_p const p2cc,
	const arm_dp_opcode_t opcode, const unsigned s,
	const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm)
{
	return(_gen(p2cc, 0,
		__gen_arm_ir_dp_opcode(opcode, s)
			| __gen_rd_rn_rm(rd, rn, rm))
		);
}

static uint32_t _gen_rd_rn_x(armcc_p const p2cc, const unsigned group,
	const arm_reg_t rd, const arm_reg_t rn, const uint32_t x)
{ return(__gen(p2cc, CC_AL, group, __gen_rd_rn(rd, rn)	| x)); }

/* **** */

uint32_t armcc::add(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm)
{
	return(_gen_dp_op_s_rd_rn(&cc, 1, ARM_ADD, 0, rd, rn, __gen_arm_dp_sop_ror_i(imm, 0)));
}

armcc::armcc(void *const p2data)
{
//	LOG("p2data: 0x%016" PRIxPTR, (uintptr_t)p2data);

	ERR_NULL(p2data);
	cc.p2data = p2data;

	org_data(0);
	org_text(0);
}

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

uint32_t armcc::b(const uint32_t pat)
{ return(_gen(&cc, 5, __gen_arm_ir_b_offset(&cc, pat))); }

uint32_t armcc::bne(const uint32_t pat)
{ return(_gen_cc(&cc, CC_NE, 5, __gen_arm_ir_b_offset(&cc, pat))); }

uint32_t armcc::cmp(const arm_reg_t rn, const arm_reg_t rm)
{ return(_gen_dp_op_s_rd_rn_rm(&cc, ARM_CMP, 1, r0, rn, rm)); }

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
	*p = data;

	return(ds_dp);
}

uint32_t armcc::ldr(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat)
{
	unsigned bit_u = 0;
	const uint32_t offset = __gen_arm_ir_ldst_offset(&cc, pat, &bit_u);

	const uint32_t ir = (1 << ARM_IR_LDST_BIT_L)
		| (1 << ARM_IR_LDST_BIT_P)
		| (bit_u << ARM_IR_LDST_BIT_U);

	return(_gen_rd_rn_x(&cc, 2, rd, rn, ir + offset));
}

uint32_t armcc::mov(const arm_reg_t rd, const arm_reg_t rm)
{ return(_gen_dp_op_s_rd_rn_rm(&cc, ARM_MOV, 0, rd, r0, rm)); }

uint32_t armcc::mov(const arm_reg_t rd, const uint8_t imm)
{
	return(_gen_dp_op_s_rd_rn(&cc, 1, ARM_MOV, 0, rd, r0, __gen_arm_dp_sop_ror_i(imm, 0)));
}

uint32_t armcc::org_data(const uint32_t ds)
{
	cc.ds = ds;

	cc.p2ds = ((uint8_t*)cc.p2data) + ds;
	cc.dp = 0;

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

uint32_t armcc::str(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat)
{
	unsigned bit_u = 0;
	const uint32_t offset = __gen_arm_ir_ldst_offset(&cc, pat, &bit_u);

	const uint32_t ir = (bit_u << ARM_IR_LDST_BIT_U)
		| (1 << ARM_IR_LDST_BIT_P);

	return(_gen_rd_rn_x(&cc, 2, rd, rn, ir + offset));
}
