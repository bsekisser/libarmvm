#pragma once

/* **** */

#define fail_if(_x) ({if(_x) { LOG(">> FAILED... in \"%s\" on line: %d", __func__, __LINE__); }})

/* **** */

enum {
	r0, r1,  r2,  r3,  r4,  r5,  r6,  r7,
	r8, r9, r10, r11, r12, r13, r14, r15,
//
	rSP = 13,
	rLR = 14,
	rPC = 15,
};

#define rRx(_x) pCORE->gpr[_x]
#define rR(_x) rRx(r##_x)

#define GEN_COUNT t->gen.count
#define GEN_IP t->gen.ip
#define GEN_PC t->gen.pc

#define RUN_PC rR(PC)

#define TEST_PC t->test_pc
