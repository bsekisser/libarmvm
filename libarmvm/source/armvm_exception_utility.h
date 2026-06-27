#pragma once

/* **** */

#include "armvm_core.h"
#include "control.h"

/* **** */

#include <stdint.h>

/* **** */

static __attribute__((warn_unused_result))
uint32_t _high_vectors(armvm_core_ref core)
{
	libarmvm_ref avm = core->armvm;

	return(CONTROL(v) ? (~0xffff) : 0);
	(void)avm;
}
