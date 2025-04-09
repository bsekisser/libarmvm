#pragma once

/* **** */

typedef struct armvm_mem_tag** armvm_mem_hptr;
typedef armvm_mem_hptr const armvm_mem_href;

typedef struct armvm_mem_tag* armvm_mem_ptr;
typedef armvm_mem_ptr const armvm_mem_ref;

typedef struct armvm_mem_callback_tag** armvm_mem_callback_hptr;
typedef armvm_mem_callback_hptr const armvm_mem_callback_href;

typedef struct armvm_mem_callback_tag* armvm_mem_callback_ptr;
typedef armvm_mem_callback_ptr const armvm_mem_callback_ref;

#include <stddef.h>
#include <stdint.h>

typedef uint32_t (*armvm_mem_fn)(void *const param, const uint32_t ppa, const size_t size, uint32_t *const write);

#include "armvm.h"

#include <stdlib.h>

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/log.h"

/* **** */

typedef struct armvm_mem_callback_tag {
	armvm_mem_fn fn;
	void* param;
}armvm_mem_callback_t;

/* **** */

void armvm_mem(armvm_mem_ref mem, action_ref action);
armvm_mem_ptr armvm_mem_alloc(armvm_ref avm, armvm_mem_href h2mem);

uint32_t armvm_mem_access_read(armvm_mem_ref mem,
	const uint32_t ppa, const size_t size,
	armvm_mem_callback_href h2cb);

armvm_mem_callback_ptr armvm_mem_access_write(armvm_mem_ref mem,
	const uint32_t ppa, const size_t size,
	const uint32_t write);

uint32_t armvm_mem_generic_page_ro(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

uint32_t armvm_mem_generic_page_rw(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

void armvm_mem_mmap_cb(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end,
	armvm_mem_fn const fn, void *const param);

void armvm_mem_mmap_ro(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end, void *const data);

void armvm_mem_mmap_rw(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end, void *const data);

/* **** */

static inline uint32_t armvm_mem_callback_io(armvm_mem_callback_ref cb,
	const uint32_t ppa, const size_t size,
	uint32_t* write)
{
	if(!cb) return(0);
	if(!cb->fn) return(0);

	return(cb->fn(cb->param, ppa, size, write));
}
