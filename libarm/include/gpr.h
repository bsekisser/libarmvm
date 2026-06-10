#pragma once

/* **** */

typedef enum gpr_enum {
	rR0, rR1, rR2, rR3, rR4, rR5, rR6, rR7,
	rR8, rR9,rR10,rR11,rR12,rR13,rR14,rR15,
//
	rLR = rR14, rRLR = rLR,
	rPC = rR15, rRPC = rPC,
	rSP = rR13, rRSP = rSP,
}gpr_enum;
typedef gpr_enum const gpr_eref;
