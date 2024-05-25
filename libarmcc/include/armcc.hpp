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

typedef uint32_t arm_sop_t;

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
		uint32_t add(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm);
//		armcc();
		armcc(void *const p2data);
		armcc(void *const p2data, armcc_h h2armcc_t);
		armcc(void *const p2data, const uint32_t cs, const uint32_t ds);
		uint32_t b(const uint32_t pat);
		uint32_t b(const arm_condition_t cc, const uint32_t pat);
		uint32_t bne(const uint32_t pat);
		uint32_t cmp(const arm_reg_t rn, const arm_reg_t rm);
		uint32_t dw(const uint32_t data);
		uint32_t ldr(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat);
		uint32_t mov(const arm_reg_t rd, const arm_reg_t rm);
		uint32_t mov(const arm_reg_t rd, const uint8_t imm);
		uint32_t org_text(const uint32_t cs);
		uint32_t org_data(const uint32_t ds);
		armcc_p p2armcc_t(void);
		uint32_t str(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat);
};
