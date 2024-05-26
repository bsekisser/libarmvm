#pragma once

/* **** */

#include "armcc_t.h"

/* **** */

extern "C" {
	#include "libarm/include/arm_cc.h"
}

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t armcc_sop_t;

enum arm_reg_t {
	r0, r1, r2, r3, r4, r5, r6, r7,
	r8, r9, r10, r11, r12, r13, r14, r15,
//
	rLR = r14,
	rPC = r15,
	rSP = r13,
};

class armcc {
	private:
		armcc_t cc;
	public:
		uint32_t adcs(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
		uint32_t add(const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop);
		uint32_t add(const arm_reg_t rd, const arm_reg_t rn, const int imm);
		uint32_t adds(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
		uint32_t adds(const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop);
		uint32_t adds(const arm_reg_t rd, const arm_reg_t rn, const int imm);
		uint32_t ands(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
//		armcc();
		armcc(void *const p2data);
		armcc(void *const p2data, armcc_h h2armcc_t);
		armcc(void *const p2data, const uint32_t cs, const uint32_t ds);
		armcc_sop_t asr(const arm_reg_t rm, const uint8_t rs);
		uint32_t asrs(const arm_reg_t rd, const arm_reg_t rm, const uint8_t rs);
		uint32_t b(const uint32_t pat);
		uint32_t b(const arm_condition_t cc, const uint32_t pat);
		uint32_t bics(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
		uint32_t bl(const uint32_t pat);
		uint32_t blx(const arm_reg_t rm);
		uint32_t blx(const uint32_t pat);
		uint32_t bx(const arm_reg_t rm);
		uint32_t bne(const uint32_t pat);
		uint32_t cmp(const arm_reg_t rn, const arm_reg_t rm);
		uint32_t dw(const uint32_t data);
		uint32_t ldr(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat);
		armcc_sop_t lsl(const arm_reg_t rm, const uint8_t rs);
		uint32_t lsls(const arm_reg_t rd, const arm_reg_t rm, const uint8_t rs);
		armcc_sop_t lsr(const arm_reg_t rm, const uint8_t rs);
		uint32_t lsrs(const arm_reg_t rd, const arm_reg_t rm, const uint8_t rs);
		uint32_t mov(const arm_reg_t rd, const arm_reg_t rm);
		uint32_t mov(const arm_reg_t rd, const int imm);
		uint32_t movs(const arm_reg_t rd, const arm_reg_t rm);
		uint32_t movs(const arm_reg_t rd, const armcc_sop_t sop);
		uint32_t org_text(const uint32_t cs);
		uint32_t org_data(const uint32_t ds);
		armcc_p p2armcc_t(void);
		armcc_sop_t ror(const arm_reg_t rm, const uint8_t rs);
		armcc_sop_t ror(const uint8_t rm, const uint8_t rs);
		uint32_t sbcs(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm);
		uint32_t sbcs(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
		uint32_t str(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat);
		uint32_t subs(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm);
		uint32_t subs(const arm_reg_t rd, const arm_reg_t rn, const arm_reg_t rm);
		uint32_t subs(const arm_reg_t rd, const arm_reg_t rn, const armcc_sop_t sop);
};
