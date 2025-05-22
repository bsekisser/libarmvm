#define pARMVM pARMVM_CORE->armvm

#define pARMVM_CORE core
#include "armvm_core_glue.h"
#include "armvm_glue.h"

#include "armvm_core_exception.h"

/* **** */

#include "armvm_core.h"
#include "armvm_coprocessor_cp15.h"
#include "armvm_exception_utility.h"
#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

#include "libbse/include/action.h"

/* **** */

int armvm_core_exception_data_abort(armvm_core_ref core)
{
	rABT(R14) = ARM_PC_NEXT;
	rABT(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(Abort));

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x10;

	return(0);
}

int armvm_core_exception_prefetch_abort(armvm_core_ref core)
{
	rABT(R14) = ARM_IP_NEXT;
	rABT(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(Abort));

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x0c;

	return(0);
}

void armvm_core_exception_reset(armvm_core_ref core)
{
	ACTION_LOG(reset);

//	rSVC(R14) = UNPREDICTABLE;
//	rSVC(SPSR) = UNPREDICTABLE;

	CPSR = ARM_CPSR_M32_BMAS(Supervisor);

	ARM_CPSR_BMAS(Thumb, 0);
	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x00;
}

int armvm_core_exception_swi(armvm_core_ref core)
{
	rSVC(R14) = ARM_PC_NEXT;
	rSVC(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(Supervisor));

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
//	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x08;

	return(0);
}

int armvm_core_exception_undefined_instruction(armvm_core_ref core)
{
	rUND(R14) = ARM_PC_NEXT;
	rUND(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(Undefined));

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
//	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x04;

	return(0);
}
