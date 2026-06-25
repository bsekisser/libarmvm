#include "apsr/apsr.h"
#include "apsr/glue.h"

/* **** */

#include <stdint.h>

/* **** */

__attribute__((visibility("default")))
uint32_t arm_apsr_read(arm_apsr_tref apsr)
{
#pragma push_macro("BX")

	#undef BX
	#define BX(_l, _u) APSR_BSET_AS(_u, APSR(_l))

	return(BX(c, C) | BX(n, N) | BX(v, V) | BX(z, Z));

#pragma pop_macro("BX")
}

__attribute__((visibility("default")))
uint32_t arm_apsr_read_masked(arm_apsr_tref apsr, const uint32_t mask)
{ return(arm_apsr_read(apsr) & mask); }

__attribute__((visibility("default")))
void arm_apsr_write(arm_apsr_ref apsr, const uint32_t v)
{
#pragma push_macro("BX")

	#undef BX
	#define BX(_l, _u) ._l = APSR_BEXT(v, _u)

	apsr->raw_flags = ((arm_apsr_t){
		.in = { BX(c, C), BX(n, N), BX(v, V), BX(z, Z) },
	}).raw_flags;

#pragma pop_macro("BX")
}
