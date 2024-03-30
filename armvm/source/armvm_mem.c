#include "armvm_mem.h"

/* **** */

#include "armvm_state.h"
#include "armvm.h"

/* **** */

#include "err_test.h"
#include "handle.h"
#include "mem_access_le.h"
#include "page.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

typedef struct armvm_mem_t {
	void* l1[PAGE_SIZE];
	armvm_mem_callback_p cb_heap[PAGE_SIZE][PAGE_SIZE];
//
	armvm_p avm;
	armvm_mem_h h2mem;
}armvm_mem_t;

/* **** */

static void armvm__mem_alloc_init(armvm_mem_p mem)
{
	if(mem->avm->config.trace.alloc_init)
		LOG();
}

static void armvm__mem_exit(armvm_mem_p mem)
{
	if(mem->avm->config.trace.exit)
		LOG();

	handle_free((void*)mem->h2mem);
}

void armvm_mem(unsigned action, armvm_mem_p mem)
{
	switch(action) {
		case ARMVM_ACTION_ALLOC_INIT: return(armvm__mem_alloc_init(mem));
		case ARMVM_ACTION_EXIT: return(armvm__mem_exit(mem));
	}
}

armvm_mem_p armvm_mem_alloc(armvm_mem_h h2mem, armvm_p avm)
{
	ERR_NULL(h2mem);
	ERR_NULL(avm);

	if(avm->config.trace.alloc)
		LOG();

	armvm_mem_p mem = handle_calloc((void*)h2mem, 1, sizeof(armvm_mem_t));
	ERR_NULL(mem);

	mem->avm = avm;

	/* **** */

	/* **** */

	return(mem);
}

uint32_t armvm_mem_generic_page_ro(uint32_t ppa, size_t size, uint32_t *const write, void *const param)
{
	uint8_t *const p = param + PAGE_OFFSET(ppa);
	return(mem_access_le(p, size, 0));
}

uint32_t armvm_mem_generic_page_rw(uint32_t ppa, size_t size, uint32_t *const write, void *const param)
{
	uint8_t *const p = param + PAGE_OFFSET(ppa);
	return(mem_access_le(p, size, write));
}

void armvm_mem_mmap(armvm_mem_p mem);
