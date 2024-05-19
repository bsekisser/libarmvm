#pragma once

/* **** */

typedef struct armcc_t* armcc_p;

/* **** */

#include <stdint.h>

/* **** */

typedef struct armcc_t {
	uint32_t cs, ip, pc;
	uint32_t ds, dp;
//
	void *p2data, *p2cs, *p2ds;
}armcc_t;

enum armcc_reg_t {
	r0, r1, r2, r3, r4, r5, r6, r7,
	r8, r9, r10, r11, r12, r13, r14, r15,
//
	rLR = r14,
	rPC = r15,
	rSP = r13,
};
