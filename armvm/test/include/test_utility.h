#define fail_if(_x) if(_x) { LOG(">> FAILED... in \"%s\" on line: %d", __FUNC__, __LINE__); return(-1) };

/* **** */

enum {
	r0, r1,  r2,  r3,  r4,  r5,  r6,  r7,
	r8, r9, r10, r11, r12, r13, r14, r15
//
	rSP = 13,
	rLR = 14,
	rPC = 15,
};

#define rRx(_x) t->avm->gpr[_x]
#define rR(_x) rRx(r##_x)
