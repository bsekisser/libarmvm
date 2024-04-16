#include "armvm_core.h"

/* **** */

#include "armvm_core_arm.h"
#include "armvm_core_config.h"
#include "armvm_core_exception.h"
#include "armvm_core_glue.h"
#include "armvm_core_thumb.h"

/* **** */

#include "armvm_action.h"
#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"

/* **** */

static void __armvm_core_alloc_init(armvm_core_p const core)
{
	core->cp = core->armvm->coprocessor;
	core->mmu = core->armvm->mmu;

	/* **** */

	if(core->armvm->config.trace.alloc) LOG();

	core->config.pedantic.ir_checks = 1;
	core->config.version = arm_v5tej;

	switch(core->config.version) {
		case arm_v5tej:
			core->config.features.thumb = 1;
		break;
	}
}

static void __armvm_core_exit(armvm_core_p const core)
{
	if(core->armvm->config.trace.exit) LOG();

	handle_free((void*)core->h2core);
}

static void __armvm_core_psr_swap_reg(uint32_t* dst, uint32_t* src)
{
	const uint32_t tmp = *dst;
		*dst = *src;
		*src = tmp;
}

/* **** */

static uint32_t* _armvm_core_psr_mode_regs(armvm_core_p const core,
	uint32_t mode, unsigned* reg, unsigned* swap_spsr)
{
	*reg = 13;

	if(swap_spsr) {
		switch(mode) {
			case ARM_CPSR_M32(Abort):
			case ARM_CPSR_M32(FIQ):
			case ARM_CPSR_M32(IRQ):
			case ARM_CPSR_M32(Supervisor):
			case ARM_CPSR_M32(Undefined):
				*swap_spsr = 1;
			break;
			case ARM_CPSR_M32(System):
			case ARM_CPSR_M32(User):
				*swap_spsr = 0;
			break;
		}
	}

	switch(mode) {
		case ARM_CPSR_M32(Abort):
			return(&rABT(R13));
		case ARM_CPSR_M32(FIQ):
			*reg = 8;
			return(&rFIQ(R8));
		case ARM_CPSR_M32(IRQ):
			return(&rIRQ(R13));
		case ARM_CPSR_M32(Supervisor):
			return(&rSVC(R13));
		case ARM_CPSR_M32(Undefined):
			return(&rUND(R13));
		case ARM_CPSR_M32(System):
		case ARM_CPSR_M32(User):
			break;
		default:
			LOG_ACTION(exit(-1));
	}

	*reg = 0;
	return(0);
}

static void _armvm_core_psr_swap_regs(armvm_core_p const core,
	uint32_t* dst, uint32_t* src,
	unsigned r, const unsigned swap_spsr)
{
	if(!r) return;
	if(!dst) dst = &GPRx(r);
	if(!src) src = &GPRx(r);

	for(; r < 15; r++)
		__armvm_core_psr_swap_reg(dst++, src++);

	if(swap_spsr)
		pSPSR = src;
}

/* **** */

void armvm_core(armvm_core_p const core, const unsigned action)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: __armvm_core_alloc_init(core); break;
		case ARMVM_ACTION_RESET: armvm_core_exception_reset(core); break;
	}
//
//
	switch(action) {
		case ARMVM_ACTION_EXIT: __armvm_core_exit(core); break;
	}
}

armvm_core_p armvm_core_alloc(armvm_p const avm, armvm_core_h const h2core)
{
	ERR_NULL(avm);
	ERR_NULL(h2core);

	if(avm->config.trace.alloc) LOG();

	armvm_core_p core = handle_calloc((void*)h2core, 1, sizeof(armvm_core_t));
	ERR_NULL(core);

	/* **** */

	core->armvm = avm;
	core->h2core = h2core;

	/* **** */

	return(core);
}

int armvm_core_in_a_privaleged_mode(armvm_core_p const core)
{ return(0 != mlBFEXT(CPSR, 3, 0)); }

void armvm_core_psr_mode_switch(armvm_core_p const core, const uint32_t new_cpsr)
{
	const uint32_t old_mode = mlBFEXT(CPSR, 4, 0);
	const uint32_t new_mode = mlBFEXT(new_cpsr, 4, 0);

	if(old_mode == new_mode)
		return;

	pbBFINS(CPSR, new_mode, 0, 5);

	unsigned sreg = 0;
	void *dst = _armvm_core_psr_mode_regs(core, old_mode, &sreg, 0);
	_armvm_core_psr_swap_regs(core, dst, 0, sreg, 0);

	unsigned dreg = 0, swap_spsr = 0;
	void *src = _armvm_core_psr_mode_regs(core, new_mode, &dreg, &swap_spsr);
	_armvm_core_psr_swap_regs(core, 0, src, dreg, swap_spsr);
}

void armvm_core_psr_mode_switch_cpsr(armvm_core_p const core, const uint32_t new_cpsr)
{
	armvm_core_psr_mode_switch(core, new_cpsr);
	CPSR = new_cpsr;
}

void armvm_core_psr_mode_switch_cpsr_spsr(armvm_core_p const core)
{ armvm_core_psr_mode_switch_cpsr(core, armvm_core_spsr(core, 0)); }

void armvm_core_step(armvm_core_p const core)
{
	CYCLE++;
	ICOUNT++;

	if(1 & PC)
		return(armvm_core_thumb_step(core));

	return(armvm_core_arm_step(core));
}
