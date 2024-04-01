#include "git/libarm/include/arm_translate.h"

int translate_arm(armvm_p avm)
{
	IP = PC;
	PC = ARM_PC_NEXT;

	const int ifetch_rval = avm->fn.ifetch(&IR, IP, 4, avm->fn.param);
	if(1 != ifetch_rval)
		return(0);

	switch(ARM_IR_CC) {
	default: /* TODO */
		return(-1);
	__attribute__((fallthrough));
	case CC_AL:
		switch(ARM_IR_GROUP) {
			case 0: // xxxx 000x xxxx xxxx
;//				return(armvm_step0(avm));
			case 1: // xxxx 001x xxxx xxxx
;//				return(armvm_step1(avm));
			case 2: // xxxx 010x xxxx xxxx
;//				return(_arm_inst_ldst_immediate(avm));
			case 5: // xxxx 101x xxxx xxxx
;//				return(_arm_inst_b_bl(avm));
			default:
				return(__arm_decode_fail(avm));
		}
		break;
	case CC_NV:
		switch(ARM_IR_GROUP) {
			case 5: // xxxx 101x xxxx xxxx
;//				return(_arm_inst_blx(avm));
			default:
				return(__arm_decode_fail(avm));
		}
		break;
	}

	return(__arm_decode_fail(avm));
}

int translate_block(uint32_t pat, armvm_p avm)
{
	PC = pat;
	assert(0 == (PC & 1));

	while(1 == translate_arm(avm))
		;
}
