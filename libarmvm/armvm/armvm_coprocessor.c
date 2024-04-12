#include "armvm_coprocessor.h"
#include "armvm_core.h"
#include "armvm_action.h"
#include "armvm.h"

/* **** */

#include "armvm_coprocessor_glue.h"
#include "armvm_core_glue.h"

/* **** */

#include "libarm/include/arm_disasm.h"

#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"
#include "libbse/include/log.h"

/* **** */

typedef struct armvm_coprocessor_callback_t* armvm_coprocessor_callback_p;
typedef struct armvm_coprocessor_callback_t {
	armvm_coprocessor_callback_fn fn;
	void* param;
}armvm_coprocessor_callback_t;

typedef struct armvm_coprocessor_t {
	armvm_coprocessor_callback_t cp15[16][16][7][7];
//
	armvm_p armvm;
	armvm_core_p core;
//
	armvm_coprocessor_h h2cp;
}armvm_coprocessor_t;

/* **** */

static armvm_coprocessor_callback_p _armvm_coprocessor_callback(const uint32_t ir, armvm_coprocessor_p const  cp)
{
	return(&cp->cp15[ir_cp_crn(ir)][ir_cp_crm(ir)][ir_cp_op1(ir)][ir_cp_op2(ir)]);
}

static void _armvm_coprocessor_alloc_init(armvm_coprocessor_p const cp)
{
	if(cp->armvm->config.trace.alloc_init) LOG();

	cp->core = cp->armvm->core;
}

static void _armvm_coprocessor_exit(armvm_coprocessor_p const cp)
{
	if(cp->armvm->config.trace.exit) LOG();

	handle_free((void*)cp->h2cp);
}

void armvm_coprocessor(const unsigned action, armvm_coprocessor_p const cp)
{
	ERR_NULL(cp);

	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: _armvm_coprocessor_alloc_init(cp); break;
	}
//
//
	switch(action) {
		case ARMVM_ACTION_EXIT: _armvm_coprocessor_exit(cp); break;
	}
}

uint32_t armvm_coprocessor_access(uint32_t *const write, armvm_coprocessor_p const cp)
{
	armvm_core_p const core = cp->core;

	if(15 == ir_cp_num(IR)) {
		armvm_coprocessor_callback_p const cb = _armvm_coprocessor_callback(write, cp);

		if(cb->fn)
			return(cb->fn(write, cb->param));
	}

	arm_disasm_arm(IP, IR);
	LOG_ACTION(exit(-1));
	return(-1);
}


armvm_coprocessor_p armvm_coprocessor_alloc(armvm_coprocessor_h const h2cp, armvm_p const avm)
{
	ERR_NULL(h2cp);
	ERR_NULL(avm);

	if(avm->config.trace.alloc) LOG();

	armvm_coprocessor_p const cp =
		handle_calloc((void*)h2cp, 1, sizeof(armvm_coprocessor_t));

	ERR_NULL(cp);

	/* **** */

	cp->armvm = avm;
	cp->h2cp = h2cp;

	/* **** */

	return(cp);
}

void armvm_coprocessor_register_callback(uint32_t cpx,
	armvm_coprocessor_callback_fn const fn,
	void *const param, armvm_coprocessor_p const cp)
{
	if(15 == ir_cp_num(cpx)) {
		armvm_coprocessor_callback_p const cb = _armvm_coprocessor_callback(cpx, cp);

		cb->fn = fn;
		cb->param = param;
	}
}
