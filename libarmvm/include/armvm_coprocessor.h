#pragma once

/* **** */

typedef struct armvm_coprocessor_t** armvm_coprocessor_h;
typedef struct armvm_coprocessor_t* armvm_coprocessor_p;

/* **** */

#include "armvm.h"

/* **** */

#include <stdint.h>

/* **** */

typedef uint32_t (*armvm_coprocessor_callback_fn)(uint32_t* write, void* param);

/* **** */

void armvm_coprocessor(unsigned action, armvm_coprocessor_p cp);
armvm_coprocessor_p armvm_coprocessor_alloc(armvm_coprocessor_h h2cp, armvm_p avm);

void armvm_coprocessor_register_callback(uint32_t cpx,
	armvm_coprocessor_callback_fn vn,
	void* param, armvm_coprocessor_p cp);
