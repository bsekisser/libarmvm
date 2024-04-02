#include "armvm_core_exception.h"

/* **** */

#include "armvm_exception_utility.h"
#include "armvm_coprocessor_cp15.h"
#include "armvm.h"

/* **** */

#include "libarm/arm_cpsr.h"

/* **** */

int armvm_core_exception_data_abort(armvm_core_p core)
{
	R14_ABT = ARM_PC_NEXT;
	SPSR_ABT = CPSR;

	CPSR &= ~0x1f;
	ARM_CPSR_M32(Abort);

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x10;

	return(0);
}

int armvm_core_exception_prefetch_abort(armvm_core_p core)
{
	R14_ABT = ARM_IP_NEXT;
	SPSR_ABT = CPSR;

	CPSR &= ~0x1f;
	ARM_CPSR_M32(Abort);

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x0c;

	return(0);
}

void armvm_core_exception_reset(armvm_core_p core)
{
//	R14_SVC = UNPREDICTABLE;
//	SPSR_SVC = UNPREDICTABLE;

	ARM_CPSR_M32_BMAS(Supervisor);

	ARM_CPSR_BMAS(Thumb, 0);
	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x00;
}

void armvm_core_exception_swi(armvm_core_p core)
{
	R14_SVC = ARM_PC_NEXT;
	SPSR_SVC = CPSR;

	ARM_CPSR_M32_BMAS(Supervisor);

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
//	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x08;
}

void armvm_core_exception_undefined_instruction(armvm_core_p core)
{
	R14_UND = ARM_PC_NEXT;
	SPSR_SVC = CPSR;

	ARM_CPSR_M32_BMAS(Undefined);

	ARM_CPSR_BMAS(Thumb, 0);
//	ARM_CPSR_BMAS(FIQ, 1);
	ARM_CPSR_BMAS(IRQ, 1);
//	ARM_CPSR_BMAS(Abort, 1);
	ARM_CPSR_BMAS(E, CP15_REG1_BIT(EE));

	PC = _high_vectors(core) | 0x04;
}

