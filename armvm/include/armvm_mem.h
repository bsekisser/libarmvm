#pragma once

/* **** */

typedef struct armvm_mem_t** armvm_mem_h;
typedef struct armvm_mem_t* armvm_mem_p;

typedef struct armvm_mem_callback_t** armvm_mem_callback_h;
typedef struct armvm_mem_callback_t* armvm_mem_callback_p;

#include <stddef.h>
#include <stdint.h>

typedef uint32_t (*armvm_mem_fn)(uint32_t ppa, size_t size, uint32_t* write, void* param);

#include "armvm.h"

/* **** */

typedef struct armvm_mem_callback_t {
	armvm_mem_fn fn;
	void* param;
}armvm_mem_callback_t;

/* **** */

void armvm_mem(unsigned action, armvm_mem_p mem);
armvm_mem_p armvm_mem_alloc(armvm_mem_h h2mem, armvm_p avm);
uint32_t armvm_mem_generic_page_ro(uint32_t ppa, size_t size, uint32_t* write, void* param);
uint32_t armvm_mem_generic_page_rw(uint32_t ppa, size_t size, uint32_t* write, void* param);
