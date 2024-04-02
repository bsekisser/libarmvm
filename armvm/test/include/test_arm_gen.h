#pragma once

/* **** */

#include "libarmvm/include/armvm_mem.h"

/* **** */

#include "libarm/include/arm_cc.h"

/* **** */

#include "libbse/include/unused.h"

/* **** */

#include <endian.h>

/* **** */

extern test_p t;

#include "test_gen_utility.h"

/* **** */

UNUSED_FN
static uint32_t b(int32_t offset) {
	return(_gen_op(5, _gen_ir_b_offset(offset)));
}

UNUSED_FN
static uint32_t bl(int32_t offset) {
	return(_gen_op(5, _BV(24) + _gen_ir_b_offset(offset)));
}
