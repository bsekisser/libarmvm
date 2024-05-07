#pragma once

/* **** */

typedef struct armvm_coprocessor_t** armvm_coprocessor_h;
typedef struct armvm_coprocessor_t* armvm_coprocessor_p;

/* **** */

#include "armvm.h"

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t (*armvm_coprocessor_callback_fn)(void *const param, uint32_t *const write);

/* **** */

void armvm_coprocessor(armvm_coprocessor_p const cp, const unsigned action);
uint32_t armvm_coprocessor_access(armvm_coprocessor_p const cp, uint32_t *const write);
armvm_coprocessor_p armvm_coprocessor_alloc(armvm_p const avm, armvm_coprocessor_h const h2cp);
uint32_t armvm_coprocessor_cp15r(armvm_coprocessor_p const cp, const uint32_t cpx, uint32_t *const write);
void armvm_coprocessor_cp15r_bmas(armvm_coprocessor_p const cp, const uint32_t cpx,
	const unsigned bit, const unsigned set);
uint32_t* armvm_coprocessor_cp15r_rmw(armvm_coprocessor_p const cp, const uint32_t cpx);

void armvm_coprocessor_register_callback(armvm_coprocessor_p const cp,
	const uint32_t cpx,
	armvm_coprocessor_callback_fn const vn,	void *const param);

#define rCP15x(_x, _write) armvm_coprocessor_cp15r(pARMVM->coprocessor, _x, _write)
