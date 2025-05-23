#include "armvm_mem.h"

/* **** */

#include "armvm_mem_config.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"
#include "libbse/include/err_test.h"
#include "libbse/include/handle.h"
#include "libbse/include/mem_access_le.h"
#include "libbse/include/page.h"
#include "libbse/include/queue.h"
#include "libbse/include/unused.h"

/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

#define kL2HeapAlloc PAGE_SIZE

#define kL2MallocCountLimit PAGE_SIZE
#define kL2MallocAlloc (sizeof(armvm_mem_callback_t) * PTE_SIZE)

typedef struct armvm_mem_tag {
	void* l1[PTD_SIZE];
	armvm_mem_callback_t l2heap[kL2HeapAlloc][PTE_SIZE];
//
	queue_t l2free;
	struct {
		void* ptr[kL2MallocAlloc];
		unsigned count;
	}l2malloc;
//
	armvm_ptr avm;
	armvm_mem_config_t config;
}armvm_mem_t;

/* **** */

static void __armvm_mem_alloc_init(armvm_mem_ref mem)
{
	ACTION_LOG(alloc_init);
	ERR_NULL(mem);

	return;
	UNUSED(mem);
}

static void __armvm_mem_exit(armvm_mem_ref mem)
{
	ACTION_LOG(exit);

	handle_ptrfree(mem);
}

/* **** */

static void* _armvm_mem_access_l1(armvm_mem_ref mem,
	const uint32_t ppa, void** *const h2l1e)
{
	void* *const p2l1e = &mem->l1[PTD(ppa)];
	void* const p2l2 = *p2l1e;

	if(h2l1e) *h2l1e = p2l1e;

	return(p2l2);
}

static armvm_mem_callback_ptr _armvm_mem_access_l2(armvm_mem_ref mem,
	const uint32_t ppa, void *const p2l2)
{
	armvm_mem_callback_ref l2 = p2l2;

	return(&l2[PTE(ppa)]);
	UNUSED(mem);
}

static armvm_mem_callback_ptr _armvm_mem_access(armvm_mem_ref mem,
	const uint32_t ppa, void** *const h2l1e)
{
	void *const p2l2 = _armvm_mem_access_l1(mem, ppa, h2l1e);

	if(!p2l2) return(0);

	return(_armvm_mem_access_l2(mem, ppa, p2l2));
}

static armvm_mem_callback_ptr _armvm_mem_mmap_alloc_free(armvm_mem_ref mem)
{
	qelem_p const qel = mem->l2free.head;
	armvm_mem_callback_ref p2l2 = (armvm_mem_callback_ptr)qel;

	if(qel) {
		qelem_p const next = qel->next;

		if(mem->config.trace.mmap.alloc.free) {
			LOG_START(">> qel: 0x%016" PRIxPTR, (uintptr_t)qel);
			LOG_END(", next: 0x%016" PRIXPTR, (uintptr_t)next);
		}

		mem->l2free.head = next;
		if(!next)
			mem->l2free.tail = 0;
	}

	return(p2l2);
}

static armvm_mem_callback_ptr _armvm_mem_mmap_alloc_malloc(armvm_mem_ref mem)
{
	armvm_mem_callback_ref p2l2 = malloc(kL2MallocAlloc);

	const unsigned count = mem->l2malloc.count;

	assert(kL2MallocCountLimit > count);

	mem->l2malloc.ptr[count] = p2l2;
	mem->l2malloc.count++;

	if(mem->config.trace.mmap.alloc.malloc) {
		LOG_START(">> p2l2: 0x%016" PRIxPTR, (uintptr_t)p2l2);
		_LOG_(", size: 0x%08zx", kL2MallocAlloc);
		LOG_END(", count: 0x%08x", count);
	}

	return(p2l2);
}

static armvm_mem_callback_ptr _armvm_mem_mmap_alloc(armvm_mem_ref mem,
	const unsigned ppa)
{
	const unsigned l1ptd = PTD(ppa);
	const unsigned l2pte = PTE(ppa);
	const unsigned offset = PAGE_OFFSET(ppa);

	if(0)
		LOG("ppa: 0x%08x -- %03x:%03x:%03x", ppa, l1ptd, l2pte, offset);

	void** p2l1e = 0;
	armvm_mem_callback_ptr p2l2 = _armvm_mem_access(mem, ppa, &p2l1e);

	if(!p2l2) {
		p2l2 = _armvm_mem_mmap_alloc_free(mem);

		if(!p2l2)
			p2l2 = _armvm_mem_mmap_alloc_malloc(mem);

		if(mem->config.trace.mmap_alloc) {
			LOG_START("l2size: 0x%08zx", kL2MallocAlloc);
			_LOG_(", p2l1e: 0x%016" PRIxPTR, (uintptr_t)p2l1e);
			LOG_END(", l2: 0x%016" PRIxPTR, (uintptr_t)p2l2);
		}

		if(!p2l2) return(0);

		*p2l1e = p2l2;
		memset(p2l2, 0, kL2MallocAlloc);

		p2l2 = &p2l2[l2pte];
	}

	return(p2l2);
}

void armvm_mem(armvm_mem_ref mem, action_ref action)
{
	switch(action) {
		case _ACTION_ALLOC_INIT: return(__armvm_mem_alloc_init(mem));
		case _ACTION_EXIT: return(__armvm_mem_exit(mem));
		default: break;
	}
}

uint32_t armvm_mem_access_read(armvm_mem_ref mem, const uint32_t ppa, const size_t size,
	armvm_mem_callback_href h2cb)
{
	armvm_mem_callback_ref cb = _armvm_mem_access(mem, ppa, 0);

	if(h2cb) *h2cb = cb;

	return(armvm_mem_callback_io(cb, ppa, size, 0));
}

armvm_mem_callback_ptr armvm_mem_access_write(armvm_mem_ref mem, const uint32_t ppa, const size_t size,
	uint32_t write)
{
	armvm_mem_callback_ref cb = _armvm_mem_access(mem, ppa, 0);

	armvm_mem_callback_io(cb, ppa, size, &write);

	return(cb);
}

armvm_mem_ptr armvm_mem_alloc(armvm_ref avm, armvm_mem_href h2mem)
{
	ACTION_LOG(alloc);

	ERR_NULL(h2mem);
	ERR_NULL(avm);

	armvm_mem_ref mem = handle_calloc(h2mem, 1, sizeof(armvm_mem_t));
	ERR_NULL(mem);

	avm->config.mem = &mem->config;

	mem->avm = avm;

	if(0) {
		mem->config.trace_mmap = 1;
		mem->config.trace.mmap_alloc = 1;
		mem->config.trace.mmap.alloc.free = 1;
		mem->config.trace.mmap.alloc.malloc = 1;
	}

	/* **** */

	memset(&mem->l2malloc, 0, sizeof(mem->l2malloc));

	queue_init(&mem->l2free);

	for(unsigned i = 0; i < kL2HeapAlloc; i++)
		queue_enqueue((qelem_p)&mem->l2heap[i][0], &mem->l2free);

	/* **** */

	return(mem);
}

uint32_t armvm_mem_generic_page_ro(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write)
{
	uint8_t *const p = param + ppa;
	return(mem_access_le(p, size, 0));
	UNUSED(write);
}

uint32_t armvm_mem_generic_page_rw(void *const param,
	const uint32_t ppa, const size_t size,
	uint32_t *const write)
{
	uint8_t *const p = param + ppa;
	return(mem_access_le(p, size, write));
}

static
void armvm_mem_mmap(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end,
	armvm_mem_fn const fn, void *const param)
{
	const uint32_t start = base & PAGE_MASK;
	const uint32_t stop = end & PAGE_MASK;

	uint8_t *const data_offset = param - base;

	for(uint32_t ppa = start; ppa < stop; ppa += PAGE_SIZE) {
		armvm_mem_callback_ref cb = _armvm_mem_mmap_alloc(mem, ppa);

		if(mem->config.trace_mmap) {
			LOG_START(">> cb: 0x%016" PRIxPTR, (uintptr_t)cb);
			_LOG_(", ppa: 0x%08x", ppa);
			_LOG_(", fn: 0x%016" PRIxPTR, (uintptr_t)fn);
			LOG_END(", param: 0x%016" PRIxPTR, (uintptr_t)param);
		}

		if(((armvm_mem_fn)~0U) == fn) {
			cb->fn = armvm_mem_generic_page_ro;
			cb->param = data_offset;
		} else if(((armvm_mem_fn)0U) == fn) {
			cb->fn = armvm_mem_generic_page_rw;
			cb->param = data_offset;
		} else {
			cb->fn = fn;
			cb->param = param;
		}
	}
}

void armvm_mem_mmap_cb(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end,
	armvm_mem_fn const fn, void *const param)
{ return(armvm_mem_mmap(mem, base, end, fn, param)); }

void armvm_mem_mmap_ro(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end, void *const data)
{ return(armvm_mem_mmap(mem, base, end, (void*)~0U, data)); }

void armvm_mem_mmap_rw(armvm_mem_ref mem,
	const uint32_t base, const uint32_t end, void *const data)
{ return(armvm_mem_mmap(mem, base, end, 0, data)); }
