#pragma once

/* **** */

enum arm_condition_t {
	CC_EQ, CC_NE,
	CC_CS, CC_CC,
	CC_MI, CC_PL,
	CC_VS, CC_VC,
	CC_HI, CC_LS,
	CC_GE, CC_LT,
	CC_GT, CC_LE,
	CC_AL, CC_NV,
//
	CC_HS = CC_CS,
	CC_LO = CC_CC,
};

enum {
	CC_EQ_NE = CC_EQ,
	CC_CS_CC = CC_CS,
	CC_MI_PL = CC_MI,
	CC_VS_VC = CC_VS,
	CC_HI_LS = CC_HI,
	CC_GE_LT = CC_GE,
	CC_GT_LE = CC_GT,
	CC_AL_NV = CC_AL,
};
