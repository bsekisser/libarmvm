#include "armvm_core.h"

/* **** */

#include "armvm_core_arm.h"
#include "armvm_core_config.h"
#include "armvm_core_exception.h"
#include "armvm_core_glue.h"
#include "armvm_core_thumb.h"

/* **** */

#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"

/* **** */

static void __armvm_core_psr_swap_reg(uint32_t* dst, uint32_t* src)
{
	const uint32_t tmp = *dst;
		*dst = *src;
		*src = tmp;
}

/* **** */

static uint32_t* _armvm_core_psr_mode_regs(armvm_core_ref core,
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

static void _armvm_core_psr_swap_regs(armvm_core_ref core,
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

static
int armvm_core_action_alloc_init(int err, void *const param, action_ref)
{
	ACTION_LOG(alloc_init);
	ERR_NULL(param);

	armvm_core_ref core = param;

	/* **** */

	ERR_NULL(core->armvm);

	armvm_ref armvm = core->armvm;

	ERR_NULL(core->cp = armvm->coprocessor);
	ERR_NULL(core->mmu = armvm->mmu);

	/* **** */

	core->config.pedantic.ir_checks = 1;
	core->config.version = arm_v5tej;

	switch(core->config.version) {
		case arm_v5tej:
			core->config.features.thumb = 1;
		break;
	}

	/* **** */

	return(err);
}

static
int armvm_core_action_exit(int err, void *const param, action_ref)
{
	ACTION_LOG(exit);

	/* **** */

	handle_ptrfree(param);

	/* **** */

	return(err);
}

static
int armvm_core_action_reset(int err, void *const param, action_ref)
{
	ACTION_LOG(reset);

	/* **** */

	armvm_core_exception_reset(param);

	/* **** */

	return(err);
}

armvm_core_ptr armvm_core_alloc(armvm_ref avm, armvm_core_href h2core)
{
	ACTION_LOG(alloc);

	ERR_NULL(avm);
	ERR_NULL(h2core);

	armvm_core_ref core = handle_calloc(h2core, 1, sizeof(armvm_core_t));
	ERR_NULL(core);

	/* **** */

	core->armvm = avm;

	/* **** */

	return(core);
}

int armvm_core_in_a_privaleged_mode(armvm_core_ref core)
{ return(0 != mlBFEXT(CPSR, 3, 0)); }

int armvm_core_pcx(armvm_core_ref core, const uint32_t new_pc)
{
	const unsigned set_thumb = core->config.features.thumb;

	const unsigned thumb = set_thumb ? (new_pc & 1) : 0;

	PC = new_pc & ~(3 >> thumb);

	ARM_CPSR_BMAS(Thumb, thumb);

	return(0);
}

int armvm_core_pcx_v5(armvm_core_ref core, const uint32_t new_pc)
{
	if(arm_v5t <= core->config.version)
		return(armvm_core_pcx(core, new_pc));

	return(0);
}

void armvm_core_psr_mode_switch(armvm_core_ref core, const uint32_t new_cpsr)
{
	const uint32_t old_mode = ARM_CPSR_M(32) | mlBFEXT(CPSR, 4, 0);
	const uint32_t new_mode = ARM_CPSR_M(32) | mlBFEXT(new_cpsr, 4, 0);

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

void armvm_core_psr_mode_switch_cpsr(armvm_core_ref core, const uint32_t new_cpsr)
{
	armvm_core_psr_mode_switch(core, new_cpsr);
	CPSR = ARM_CPSR_M(32) | new_cpsr;
}

void armvm_core_psr_mode_switch_cpsr_spsr(armvm_core_ref core)
{ armvm_core_psr_mode_switch_cpsr(core, armvm_core_spsr(core, 0)); }

uint32_t armvm_core_reg_user(armvm_core_ref core, const unsigned r, uint32_t *const v)
{
	unsigned reg = 0;

	const unsigned mode = mlBFEXT(CPSR, 4, 0);
	uint32_t *const usr_regs = _armvm_core_psr_mode_regs(core, mode, &reg, 0);

	uint32_t vout = 0;
	if(reg && ((r & 0x0f) < 15) && r >= reg)
	{
		const unsigned umreg = r - reg;
		vout = usr_regs[umreg];
		if(v)
			usr_regs[umreg] = *v;
	}
	else
	{
		vout = GPRx(r);
		if(v)
			GPRx(r) = *v;
	}
	return(vout);
}

uint32_t armvm_core_spsr(armvm_core_ref core, uint32_t *const write)
{
	const uint32_t data = core->spsr ? *core->spsr : (write ? *write : 0);

	if(write && core->spsr)
		*core->spsr = *write;

	return(data);
}

int armvm_core_step(armvm_core_ref core)
{
	CYCLE++;
	ICOUNT++;

	if(ARM_CPSR_BEXT(Thumb))
		return(armvm_core_thumb_step(core));

	return(armvm_core_arm_step(core));
}

int armvm_core_threaded_run(armvm_core_ref core)
{
	int rval = 0;

	while(!core->flags.halt)
		if(0 > (rval = armvm_core_step(core)))
			break;

	return(rval);
}

action_list_t armvm_core_action_list = {
	.list = {
		[_ACTION_ALLOC_INIT] = {{ armvm_core_action_alloc_init }, { 0 }, 0 },
		[_ACTION_EXIT] = {{ armvm_core_action_exit }, { 0 }, 0 },
		[_ACTION_RESET] = {{ armvm_core_action_reset }, { 0 }, 0 },
	}
};
