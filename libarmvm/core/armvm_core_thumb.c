#include "armvm_core_glue.h"

/* **** */

#include "armvm_core_exception.h"
#include "armvm_core_mem.h"
#include "armvm_core_reg_trace.h"
#include "armvm_core_thumb.h"

/* **** */

#include "armvm.h"

/* **** */

#include "libarm/include/arm_disasm.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <stdlib.h>

/* **** */

static void __thumb_fail_decode(armvm_core_p const core)
{
	for(unsigned lsb = 13; lsb > 8; lsb--) {
		uint32_t group = mlBFTST(IR, 15, lsb);
		LOG("IP[15:%2u]: 0x%08x", lsb, group);
	}

	arm_disasm_thumb(IP, IR);
	LOG_ACTION(exit(-1));
}

static void armvm_core_thumb___ascm_rd_i(armvm_core_p const core)
{
	const uint8_t operation = mlBFEXT(IR, 12, 11);

	const uint32_t rm = setup_rR_vR(core, ARMVM_TRACE_R(M), ~0U, mlBFEXT(IR, 7, 0));

	const unsigned rRN = mlBFEXT(IR, 10, 8);

	const alubox_fn _alubox_fn[4] = {
		_alubox_thumb_movs, _alubox_thumb_cmps,
			_alubox_thumb_adds, _alubox_thumb_subs };

	_alubox_fn[operation](core, &rRN, rRN, rm);

	const char* ops[4] = { "movs", "cmps", "adds", "subs" };
	const char opc[4] = { '=', '-', '+', '-' };

	switch(operation)
	{
		default:
			CORE_TRACE("%s(%s, 0x%03x); /* 0x%08x %c 0x%03x = 0x%08x */",
				ops[operation], rR_NAME(D),	vR(M),
					vR(N), opc[operation], vR(M), vR(D));
			break;
		case THUMB_ASCM_OP_MOV:
			CORE_TRACE("movs(%s, 0x%03x);", rR_NAME(D), vR(M));
			break;
	}

}

void armvm_core_thumb__step_group0(armvm_core_p const core)
{
}

void armvm_core_thumb_step(armvm_core_p const core)
{
	IP = setup_rR_vR(core, ARMVM_TRACE_R(IP), ~0, PC); // STUPID KLUDGE!!
	PC = THUMB_IP_NEXT;

	if(0 > armvm_core_mem_ifetch(core, &IR, IP, 2))
		return;

	setup_rR_vR(core, ARMVM_TRACE_R(IR), ~0, IR); // STUPID KLUDGE!!!

	const uint32_t group = mlBFTST(IR, 15, 13);
	switch(group) {
		case 0x2000: /* 001x xxxx xxxx xxxx */
			return(armvm_core_thumb_ascm_rd_i(core));
		default:
			LOG("IP[15:13]: 0x%08x", group);
	}

	return(__thumb_fail_decode(core));
}
