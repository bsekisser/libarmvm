#include "apsr/alubox.h"

/* **** */

#include "apsr/alubox_flags.h"
#include "apsr/apsr.h"
#include "dp.h"
#include "sop.h"

/* **** */

#include "libbse/include/log.h"

/* **** */

#include <stdint.h>
#include <stdlib.h>

/* **** */

__attribute__((visibility("default")))
uint32_t arm_apsr_alubox(arm_apsr_ref apsr, arm_dp_opcode_eref opcode, const uint32_t rn, const uint32_t sop)
{
	switch(opcode) {
		case ARM_ADC:
			return(flags_adc(apsr, rn, sop));
		case ARM_ADD: case ARM_CMN:
			return(flags_add(apsr, rn, sop));
		case ARM_AND: case ARM_TST:
			return(flags_nz(apsr, rn & sop));
		case ARM_BIC:
			return(flags_nz(apsr, rn & ~sop));
		case ARM_CMP: case ARM_SUB:
			return(flags_sub(apsr, rn, sop));
		case ARM_EOR: case ARM_TEQ:
			return(flags_nz(apsr, rn ^ sop));
		case ARM_ORR:
			return(flags_nz(apsr, rn | sop));
		case ARM_MOV:
			return(flags_nz(apsr, sop));
		case ARM_MVN:
			return(flags_nz(apsr, ~sop));
		case ARM_RSB:
			return(flags_sub(apsr, sop, rn));
		case ARM_RSC:
			return(flags_sbc(apsr, sop, rn));
		case ARM_SBC:
			return(flags_sbc(apsr, rn, sop));
	}

	LOGu(opcode);
	LOG_ACTION(exit(-1));
	return(0xdeadbeef);
}
