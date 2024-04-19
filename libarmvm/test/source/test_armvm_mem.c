#include "test_armvm_mem.h"
#include "test.h"

/* **** */

#include "libarmvm/include/armvm_mem.h"

#include "libbse/include/page.h"

/* **** */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

/* **** */

int test_armvm_mem(test_p t)
{
	assert(PTD_SIZE > PTD(~0U));
	assert(PTE_SIZE > PTE(~0U));

	LOG("kTEST_MEM_ALLOC: 0x%08x", kTEST_MEM_ALLOC);

	for(uint32_t ppa = 0; ppa < kTEST_MEM_ALLOC; ppa += PAGE_SIZE) {
		uint32_t* p = (void*)&t->mem[ppa];
		*p++ = ppa;
		*p++ = PTD(ppa);
		*p++ = PTE(ppa);
		*p++ = PAGE_OFFSET(ppa);
	}

	for(uint32_t ppa = 0; ppa < kTEST_MEM_ALLOC; ppa += PAGE_SIZE) {
		armvm_mem_callback_p p2cb;

		uint32_t* xp = (void*)&t->mem[ppa];
		const uint32_t xppa = armvm_mem_access_read(t->armvm->mem, ppa, 4, &p2cb);
		uint32_t* p = p2cb->param + ppa;

//		LOG("xp: 0x%016" PRIxPTR ", p: 0x%016" PRIxPTR ", ppa: 0x%08x, xppa: 0x%08x",
//			(uintptr_t)xp, (uintptr_t)p, ppa, xppa);

		assert(xppa == ppa);
		assert(xp == p);
		assert(*p++ == ppa);
		assert(*p++ == PTD(ppa));
		assert(*p++ == PTE(ppa));
		assert(*p++ == PAGE_OFFSET(ppa));
	}

	return(0);
}
