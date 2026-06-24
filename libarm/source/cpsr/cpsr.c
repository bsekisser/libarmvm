#include "cpsr/cpsr.h"
#include "cpsr/glue.h"

#include "apsr/apsr.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((visibility("default")))
uint32_t _arm_cpsr_read(arm_cpsr_tref cpsr)
{
#pragma push_macro("BX")

	#undef BX
	#define BX(_l, _u) CPSR_BSET_AS(_u, CPSR(_l))

	return(BX(abort, A) | BX(endian, E) | BX(fiq, F) | BX(irq, I) | BX(thumb, T) | CPSR(mode));

#pragma pop_macro("BX")
}

__attribute__((visibility("default")))
void _arm_cpsr_write(arm_cpsr_ref cpsr, const uint32_t v)
{
#pragma push_macro("BX")

	#undef BX
	#define BX(_l, _u) ._l = CPSR_BEXT(v, _u)

	cpsr->raw_flags = ((arm_cpsr_t){
		BX(abort, A), BX(endian, E), BX(fiq, F), BX(irq, I), BX(thumb, T), .mode = (v & 31)
	}).raw_flags;

#pragma pop_macro("BX")
}

__attribute__((visibility("default")))
uint32_t arm_cpsr_read(arm_apsr_tref apsr, arm_cpsr_tref cpsr)
{ return(arm_apsr_read(apsr) | _arm_cpsr_read(cpsr)); }

__attribute__((visibility("default")))
void arm_cpsr_write(arm_apsr_ref apsr, arm_cpsr_ref cpsr, const uint32_t v)
{ arm_apsr_write(apsr, v), _arm_cpsr_write(cpsr, v); }
