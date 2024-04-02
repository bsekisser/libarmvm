#include "test.h"

/* **** */

#include "test_arm.h"
#include "test_utility.h"

/* **** */

#include "libarmvm/include/armvm_core.h"
#include "libarmvm/include/armvm_mem.h"

/* **** */

test_t tt, *t = &tt;

int main(int argc, char** argv)
{
	armvm_alloc_init(armvm_alloc(&pARMVM));

	armvm_mem_mmap(0, sizeof(t->mem), armvm_mem_generic_page_rw, t->mem,
		pARMVM->mem);

	test_arm(t);

	armvm_exit(pARMVM);
}

uint32_t run_test(test_p t)
{
	TEST_PC = RUN_PC;

	for(; GEN_COUNT; GEN_COUNT--)
		armvm_core_step(pCORE);

//	GEN_COUNT = 0;
	GEN_PC = RUN_PC;
	return(RUN_PC);
}
