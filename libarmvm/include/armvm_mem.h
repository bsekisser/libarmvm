#pragma once

/* **** */

typedef struct armvm_mem_t** armvm_mem_h;
typedef struct armvm_mem_t* armvm_mem_p;

typedef struct armvm_mem_callback_t** armvm_mem_callback_h;
typedef struct armvm_mem_callback_t* armvm_mem_callback_p;

#include <stddef.h>
#include <stdint.h>

typedef uint32_t (*armvm_mem_fn)(void *const param, const uint32_t ppa, const size_t size, uint32_t *const write);

#include "armvm.h"

#include <stdlib.h>

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/log.h"

/* **** */

typedef struct armvm_mem_callback_t {
	armvm_mem_fn fn;
	void* param;
}armvm_mem_callback_t;

/* **** */

void armvm_mem(armvm_mem_p const mem, action_ref action);
armvm_mem_p armvm_mem_alloc(armvm_p const avm, armvm_mem_h const h2mem);

uint32_t armvm_mem_access_read(armvm_mem_p const mem,
	const uint32_t ppa, const size_t size,
	armvm_mem_callback_h const h2cb);

armvm_mem_callback_p armvm_mem_access_write(armvm_mem_p const mem,
	const uint32_t ppa, const size_t size,
	const uint32_t write);

uint32_t armvm_mem_generic_page_ro(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

uint32_t armvm_mem_generic_page_rw(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

void armvm_mem_mmap(armvm_mem_p const mem,
	const uint32_t base, const uint32_t end,
	armvm_mem_fn const fn, void *const param);

/* **** */

static inline uint32_t armvm_mem_callback_io(armvm_mem_callback_p const cb,
	const uint32_t ppa, const size_t size,
	uint32_t* write)
{
	if(!cb) return(0);
	if(!cb->fn) return(0);

	return(cb->fn(cb->param, ppa, size, write));
}
