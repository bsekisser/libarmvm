#pragma once

/* **** */

extern "C" {
	#include "libarm/include/arm_cc.h"
}

/* **** */

#include <stdint.h>

/* **** */

#include "armcc_t.h"
#include "arm_reg_t.h"

class armcc {
	private:
		armcc_t cc;
	public:
		uint32_t add(const arm_reg_t rd, const arm_reg_t rn, const uint8_t imm);
//		armcc();
		armcc(void *const p2data);
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
		uint32_t str(const arm_reg_t rd, const arm_reg_t rn, const uint32_t pat);
};
