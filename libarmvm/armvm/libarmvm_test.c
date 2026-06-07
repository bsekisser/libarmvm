#define pCORE avm->core

#include "libarmvm_test.h"

#include "armvm.h"

/* **** */

#include <assert.h>
#include <stdint.h>

/* **** */

PUBLIC
uint32_t * libarmvm_test_p2gpr(libarmvm_ref avm, const unsigned r)
{
	assert(0 == (r & ~15));
	return(&GPRx(r));
}
