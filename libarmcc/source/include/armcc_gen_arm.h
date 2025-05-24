#pragma once

/* **** */

#include "armcc_gen.h"
#include "armcc_gen_arm_ir.h"
#include "armcc_t.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/log.h"

/* **** */

#include <assert.h>
#include <stdint.h>

/* **** */

static uint32_t _gen_arm__byte_offset(const uint32_t pc, const uint32_t pat)
{ return(-8 + (pat - pc)); }

static uint32_t _gen_arm__word_offset(const uint32_t pc, const uint32_t pat)
{ return(_ASR(_gen_arm__byte_offset(pc, pat), 2)); }

/* **** */

static uint32_t gen_arm__blx_bx_rm(const unsigned link, const arm_reg_t rm)
{ return(0x012fff10 | BMOV(!!link, 0, 5) | gen_arm_ir__rm(rm)); }

static uint32_t gen_arm__b_offset(armcc_p p2cc, const uint32_t pat)
{
	const uint32_t cs_pc = p2cc->cs + p2cc->pc;

	if(0) LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);

	const uint32_t offset = _gen_arm__word_offset(cs_pc, pat);
	const uint32_t offset_masked = offset & pbBF(0, 24);

	if(0) {
		LOG_START("offset: 0x%08x", offset);
		LOG_END(", offset_masked: 0x%08x", offset_masked);
	}

	assert(offset == pbBFEXTs(offset_masked, 0, 24));

	return(offset_masked);
}

static uint32_t gen_arm__cc_group_ir(armcc_p const p2cc, const arm_condition_t cc,
	const unsigned group, const uint32_t ir_in)
{
	const uint32_t ir = gen_arm_ir__cc(cc)
		| gen_arm_ir__group(group)
		| ir_in;

	return(armcc__gen(p2cc, ir)); }

static uint32_t gen_arm__group_ir(armcc_p const p2cc, const unsigned group, const uint32_t ir)
{ return(gen_arm__cc_group_ir(p2cc, CC_AL, group, ir)); }

static uint32_t gen_arm__group_rd_rn_x(armcc_p const p2cc, const unsigned group,
	const arm_reg_t rd, const arm_reg_t rn, const uint32_t x)
{ return(gen_arm__group_ir(p2cc, group, gen_arm_ir__rd_rn(rd, rn) | x)); }

static uint32_t gen_arm__ldst_offset(armcc_p p2cc,
	const unsigned pat, unsigned *const bit_u)
{
	assert(p2cc);
	assert(bit_u);

	const uint32_t cs_pc = p2cc->cs + p2cc->pc;

	if(0) {
		LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);
	}

	const uint32_t offset = _gen_arm__byte_offset(cs_pc, pat);
	const uint32_t offset_masked = offset & pbBF(0, 12);

	if(0) {
		LOG_START("offset: 0x%08x", offset);
		LOG_END(", offset_masked: 0x%08x", offset_masked);
	}

//	assert(offset == pbBFEXTs(offset_masked, 0, 12));

	*bit_u = !((int32_t)offset < 0);

	return(offset_masked);
}
