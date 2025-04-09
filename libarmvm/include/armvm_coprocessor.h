#pragma once

/* **** */

typedef struct armvm_coprocessor_tag** armvm_coprocessor_hptr;
typedef armvm_coprocessor_hptr const armvm_coprocessor_href;

typedef struct armvm_coprocessor_tag* armvm_coprocessor_ptr;
typedef armvm_coprocessor_ptr const armvm_coprocessor_ref;

/* **** */

#include "armvm.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t (*armvm_coprocessor_callback_fn)(void *const param, uint32_t *const write);

/* **** */

void armvm_coprocessor(armvm_coprocessor_ref cp, action_ref action);
uint32_t armvm_coprocessor_access(armvm_coprocessor_ref cp, uint32_t *const write);
armvm_coprocessor_ptr armvm_coprocessor_alloc(armvm_p const avm, armvm_coprocessor_href h2cp);
uint32_t armvm_coprocessor_cp15r(armvm_coprocessor_ref cp, const uint32_t cpx, uint32_t *const write);
void armvm_coprocessor_cp15r_bclr(armvm_coprocessor_ref cp, const uint32_t cpx,
	const unsigned bit);
void armvm_coprocessor_cp15r_bmas(armvm_coprocessor_ref cp, const uint32_t cpx,
	const unsigned bit, const unsigned set);
uint32_t* armvm_coprocessor_cp15r_rmw(armvm_coprocessor_ref cp, const uint32_t cpx);

void armvm_coprocessor_register_callback(armvm_coprocessor_ref cp,
	const uint32_t cpx,
	armvm_coprocessor_callback_fn const fn, void *const param);

#define rCP15x(_x, _write) armvm_coprocessor_cp15r(pARMVM->coprocessor, _x, _write)
