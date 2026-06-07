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

#include "libarmvm_mem.h"

#include "armvm.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/log.h"

/* **** */

typedef struct armvm_mem_callback_tag {
	libarmvm_mem_fn fn;
	void* param;
}armvm_mem_callback_t;

/* **** */

extern action_list_t armvm_mem_action_list;

//__attribute__((warn_unused_result))
armvm_mem_ptr armvm_mem_alloc(libarmvm_ref avm, armvm_mem_href h2mem);

__attribute__((warn_unused_result))
uint32_t armvm_mem_access_read(armvm_mem_ref mem,
	const uint32_t ppa, const size_t size,
	armvm_mem_callback_href h2cb);

__attribute__((warn_unused_result))
armvm_mem_callback_ptr armvm_mem_access_write(armvm_mem_ref mem,
	const uint32_t ppa, const size_t size,
	const uint32_t write);

//__attribute__((warn_unused_result))
uint32_t armvm_mem_generic_page_ro(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

//__attribute__((warn_unused_result))
uint32_t armvm_mem_generic_page_rw(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write);

/* **** */

static inline //__attribute__((warn_unused_result))
uint32_t armvm_mem_callback_io(armvm_mem_callback_ref cb,
	const uint32_t ppa, const size_t size,
	uint32_t* write)
{
	if(!cb) return(0);
	if(!cb->fn) return(0);

	return(cb->fn(cb->param, ppa, size, write));
}
