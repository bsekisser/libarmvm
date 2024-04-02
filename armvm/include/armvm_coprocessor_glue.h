#pragma once

/* **** */

#include "libbse/include/bitfield.h"

/* **** */

#define ir_cp_crm(_ir) mlBFEXT(_ir, 3, 0)
#define cp_crm(_crm) ((_crm) & 15)

#define ir_cp_crn(_ir) mlBFEXT(_ir, 19, 16)
#define cp_crn(_crn) (((_crn) & 15) << 16)

#define ir_cp_num(_ir) mlBFEXT(_ir, 11, 8)
#define cp_num(_num) (((_num) & 15) << 8)

#define ir_cp_op1(_ir) mlBFEXT(_ir, 23, 21)
#define cp_op1(_op1) (((_op1) & 7) << 21)

#define ir_cp_op2(_ir) mlBFEXT(_ir, 7, 5)
#define cp_op2(_op2) (((_op2) & 7) << 5)

#define ip_cp_rd(_ir) mlBFEXT(_ir, 15, 12)
#define cp_rd(_rd) (((_rd) & 15) << 12)

#define cp15(_op1, _n, _m, _op2) \
	(cp_num(15) | cp_op1(_op1) | cp_crn(_n) | cp_crm(_m) | cp_op2(_op2))
