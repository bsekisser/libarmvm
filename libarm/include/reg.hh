#pragma once

/* **** */

#include <stdint.h>

/* **** */

typedef enum arm_gpr_enum {
	r0, r1, r2, r3, r4, r5, r6, r7,
	r8, r9,r10,r11,r12,r13,r14,r15,
//
	rLR = r14,
	rPC = r15,
	rSP = r13,
}arm_gpr_enum;
typedef arm_gpr_enum const arm_gpr_eref;

/* **** */

typedef enum arm_reg_enum {
	rD = 16, rM, rN, rS,
	rSOP,
//
	__ARM_GPR_COUNT__, __ARM_REG_COUNT__ = __ARM_GPR_COUNT__ - 16,
}arm_reg_enum;
typedef arm_reg_enum const arm_reg_eref;

/* **** */

typedef union arm_gpr_tag {
	int32_t i32;
	uint32_t u32;
}arm_gpr_t;
typedef arm_gpr_t const arm_gpr_tref;

/* **** */

typedef class arm_reg_class* arm_reg_ptr;
typedef class arm_reg_class& arm_reg_ref;
typedef class arm_reg_class arm_reg_class;

class arm_reg_class {
public:
	uint32_t* dst(arm_reg_eref rx, arm_gpr_eref rr)
		{ return(setup_p(rx, rr, 0xfeedface)); }

	uint32_t dst_wb(arm_reg_eref rx)
		{ return((gpr[reg[rx]].u32 = gpr[rx].u32)); }

	uint32_t dst_wb_v(arm_reg_eref rx, arm_gpr_eref rr, const uint32_t v)
		{ return(setup(rx, rr, gpr[rr].u32 = v)); }

	uint32_t setup(arm_reg_eref rx, arm_gpr_eref rr, const uint32_t v)
		{ return(setup_p(rx, rr, v)[0]); }

	uint32_t* setup_p(arm_reg_eref rx, arm_gpr_eref rr, const uint32_t v)
		{ reg[rx] = rr; gpr[rx].u32 = v; return(&gpr[rx].u32); }

	uint32_t src(arm_reg_eref rx, arm_gpr_eref rr)
		{ return(setup(rx, rr, gpr[rr].u32)); }
private:
	arm_gpr_t gpr[__ARM_GPR_COUNT__];
	arm_gpr_enum reg[__ARM_REG_COUNT__];
};
