#include "armvm_trace_arm.h"
#include "armvm_trace_glue.h"
#include "armvm_trace.h"

/* **** */

#include "libarm/include/arm_cc.h"
#include "libarm/include/arm_disasm.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/err_test.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <inttypes.h>
#include <stdlib.h>

/* **** */


static void __arm_decode_fail(armvm_trace_p const atp)
{ LOG_ACTION(exit(-1)); UNUSED(atp); }

/* **** */

static void arm__trace_step0_misc(armvm_trace_p const atp)
{
	switch(mlBFTST(IR, 27, 20) | mlBFTST(IR, 7, 4)) {
//		case 0x00000090:
//		case 0x00100090: return(_arm_inst_mul(atp));
		case 0x00200090:
		case 0x00300090: return(armvm_trace_mla(atp));
		case 0x00800090:
		case 0x00900090: return(armvm_trace_umull(atp));
		case 0x01000000:
		case 0x01400000: return(armvm_trace_mrs(atp));
		case 0x01200010: return(armvm_trace_bx_m(atp));
		case 0x01200030: return(armvm_trace_blx_m(atp));
//		case 0x01600010: return(_arm_inst_clz(atp));
	}

	LOG_ACTION(return(__arm_decode_fail(atp)));
}

static void arm__trace_step0(armvm_trace_p const atp)
{
	if(BEXT(IR, 4)) {
		if(BEXT(IR, 7))
			return(arm__trace_step0_misc(atp));
		else if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(arm__trace_step0_misc(atp));
		else
			return(armvm_trace_dp(atp));
//			return(armvm_trace_dp_shift_register(atp));
	} else {
		if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
			return(arm__trace_step0_misc(atp));
		else
			return(armvm_trace_dp(atp));
//			return(_arm_dp_shift_immediate(atp));
	}

	LOG_ACTION(return(__arm_decode_fail(atp)));
}

static void arm__trace_step1_misc(armvm_trace_p const atp)
{
	LOG_ACTION(return(__arm_decode_fail(atp)));
}

static void arm__trace_step1(armvm_trace_p const atp)
{
	if((2 == mlBFEXT(IR, 24, 23)) && !ARM_IR_DP_S)
		return(arm__trace_step1_misc(atp));
	else
		return(armvm_trace_dp(atp));

	LOG_ACTION(return(__arm_decode_fail(atp)));
}

void armvm_trace(armvm_trace_p const atp)
{
	ERR_NULL(atp);

	switch(ARM_IR_CC) {
	case CC_NV:
		switch(ARM_IR_GROUP) {
		case 5: // xxxx 101x xxxx xxxx
			return(armvm_trace_blx(atp));
		}
	break;
	default:
		switch(ARM_IR_GROUP) {
		case 0: // xxxx 000x xxxx xxxx
			return(arm__trace_step0(atp));
		case 1: // xxxx 001x xxxx xxxx
			return(arm__trace_step1(atp));
		case 2: // xxxx 010x xxxx xxxx
			return(armvm_trace_ldst(atp));
		case 5: // xxxx 101x xxxx xxxx
			return(armvm_trace_b_bl(atp));
		default:
			LOG_ACTION(return(__arm_decode_fail(atp)));
		}
	break;
	}

	LOG_ACTION(return(__arm_decode_fail(atp)));
}
