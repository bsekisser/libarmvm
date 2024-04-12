#pragma once

/* **** */

typedef struct armvm_coprocessor_t** armvm_coprocessor_h;
typedef struct armvm_coprocessor_t* armvm_coprocessor_p;

/* **** */

#include "armvm.h"

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t (*armvm_coprocessor_callback_fn)(uint32_t *const write, void *const param);

/* **** */

void armvm_coprocessor(const unsigned action, armvm_coprocessor_p const cp);
uint32_t armvm_coprocessor_access(uint32_t *const write, armvm_coprocessor_p const cp);
armvm_coprocessor_p armvm_coprocessor_alloc(armvm_coprocessor_h const h2cp, armvm_p const avm);

void armvm_coprocessor_register_callback(const uint32_t cpx,
	armvm_coprocessor_callback_fn const vn,
	void *const param, armvm_coprocessor_p const cp);
