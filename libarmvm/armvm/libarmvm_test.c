#include "libarmvm_test.h"

#include "armvm.h"
#include "armvm_core_glue.h"

/* **** */

#include <assert.h>
#include <stdint.h>

/* **** */

PUBLIC
uint32_t * libarmvm_test_p2gpr(libarmvm_ref avm, const unsigned r)
{
	armvm_core_ref core = avm->core;

	assert(_ARMVM_GPR_COUNT_ > r);
	return(&GPRx(r));
}
