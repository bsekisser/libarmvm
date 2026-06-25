#define pCORE avm->core

#include "armvm_exception.h"

/* **** */

#include "armvm_coprocessor_cp15.h"
#include "armvm_exception_utility.h"
#include "armvm.h"

/* **** */

#include "libarm/include/cpsr/cpsr.h"

/* **** */

// TODO: combine/reduce

int armvm_exception_fiq(libarmvm_ref avm)
{
	armvm_core_ref core = avm->core;

	rFIQ(R14) = ARM_PC_NEXT;
	rFIQ(SPSR) = armvm_core_cpsr(core, 0);

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(FIQ));

	CPSR(thumb) = 0;
	CPSR(fiq) = 1;
	CPSR(irq) = 1;
	CPSR(abort) = 1;
	CPSR(endian) = CP15_REG1_BIT(EE);

	PC = _high_vectors(core) | 0x1c;

	return(0);
}

int armvm_exception_irq(libarmvm_ref avm)
{
	armvm_core_ref core = avm->core;

	rIRQ(R14) = ARM_PC_NEXT;
	rIRQ(SPSR) = armvm_core_cpsr(core, 0);

	armvm_core_psr_mode_switch(core, ARM_CPSR_M32(IRQ));

	CPSR(thumb) = 0;
//	CPSR(fiq) = 1;
	CPSR(irq) = 1;
	CPSR(abort) = 1;
	CPSR(endian) = CP15_REG1_BIT(EE);

	PC = _high_vectors(core) | 0x18;

	return(0);
}
