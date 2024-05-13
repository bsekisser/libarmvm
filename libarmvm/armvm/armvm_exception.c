#define pARMVM pARMVM_CORE->armvm

#define pARMVM_CORE core
#include "armvm_core_glue.h"
#include "armvm_glue.h"

#include "armvm_exception.h"

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_exception_utility.h"
#include "armvm.h"

/* **** */

#include "libarm/include/arm_cpsr.h"

/* **** */

int armvm_exception_fiq(armvm_p const avm)
{
	const armvm_core_p core = avm->core;

	rFIQ(R14) = ARM_PC_NEXT;
	rFIQ(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(FIQ));

	ARM_CPSR_BMAS(Thumb, 0);
	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x1c;

	return(0);
}

int armvm_exception_irq(armvm_p const avm)
{
	const armvm_core_p core = avm->core;

	rIRQ(R14) = ARM_PC_NEXT;
	rIRQ(SPSR) = CPSR;

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(IRQ));

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x18;

	return(0);
}
