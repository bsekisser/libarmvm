#pragma once

/* **** */

#include "armcc_t.h"

/* **** */

extern "C" {
	#include "libarm/include/cc.h"
	#include "libarm/include/gpr.h"
}

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t armcc_sop_t;
typedef armcc_sop_t armcc_sop_tref;

class armcc {
	private:
		armcc_t cc;
	public:
		uint32_t adcs(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t add(gpr_eref rd, gpr_eref rn, armcc_sop_tref sop);
		uint32_t add(gpr_eref rd, gpr_eref rn, const int imm);
		uint32_t adds(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t adds(gpr_eref rd, gpr_eref rn, armcc_sop_tref sop);
		uint32_t adds(gpr_eref rd, gpr_eref rn, const int imm);
		uint32_t ands(gpr_eref rd, gpr_eref rn, gpr_eref rm);
//		armcc();
		armcc(void *const p2data);
		armcc(void *const p2data, armcc_h h2armcc_t);
		armcc(void *const p2data, const uint32_t cs, const uint32_t ds);
		armcc_sop_t asr(gpr_eref rm, const uint8_t rs);
		armcc_sop_t asr(gpr_eref rm, gpr_eref rs);
		uint32_t asrs(gpr_eref rd, gpr_eref rm, const uint8_t rs);
		uint32_t asrs(gpr_eref rd, gpr_eref rm, gpr_eref rs);
		uint32_t b(const uint32_t pat);
		uint32_t b(arm_condition_eref cc, const uint32_t pat);
		uint32_t bics(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t bl(const uint32_t pat);
		uint32_t blx(gpr_eref rm);
		uint32_t blx(const uint32_t pat);
		uint32_t bx(gpr_eref rm);
		uint32_t bne(const uint32_t pat);
		uint32_t cmp(gpr_eref rn, gpr_eref rm);
		uint32_t dw(const uint32_t data);
		uint32_t ldr(gpr_eref rd, gpr_eref rn, const uint32_t pat);
		armcc_sop_t lsl(gpr_eref rm, const uint8_t rs);
		armcc_sop_t lsl(gpr_eref rm, gpr_eref rs);
		uint32_t lsls(gpr_eref rd, gpr_eref rm, const uint8_t rs);
		uint32_t lsls(gpr_eref rd, gpr_eref rm, gpr_eref rs);
		armcc_sop_t lsr(gpr_eref rm, const uint8_t rs);
		armcc_sop_t lsr(gpr_eref rm, gpr_eref rs);
		uint32_t lsrs(gpr_eref rd, gpr_eref rm, const uint8_t rs);
		uint32_t lsrs(gpr_eref rd, gpr_eref rm, gpr_eref rs);
		uint32_t mov(gpr_eref rd, gpr_eref rm);
		uint32_t mov(gpr_eref rd, const int imm);
		uint32_t movs(gpr_eref rd, gpr_eref rm);
		uint32_t movs(gpr_eref rd, armcc_sop_tref sop);
		uint32_t org_text(const uint32_t cs);
		uint32_t org_data(const uint32_t ds);
		armcc_p p2armcc_t(void);
		armcc_sop_t ror(gpr_eref rm, const uint8_t rs);
		armcc_sop_t ror(const uint8_t rm, const uint8_t rs);
		uint32_t rsbs(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t rsbs(gpr_eref rd, gpr_eref rn, armcc_sop_tref sop);
		uint32_t sbcs(gpr_eref rd, gpr_eref rn, const uint8_t imm);
		uint32_t sbcs(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t str(gpr_eref rd, gpr_eref rn, const uint32_t pat);
		uint32_t subs(gpr_eref rd, gpr_eref rn, const uint8_t imm);
		uint32_t subs(gpr_eref rd, gpr_eref rn, gpr_eref rm);
		uint32_t subs(gpr_eref rd, gpr_eref rn, armcc_sop_tref sop);
};
