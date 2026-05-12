#pragma once

/* **** */

typedef struct test_tag** test_hptr;
typedef test_hptr const test_href;

typedef struct test_tag* test_ptr;
typedef test_ptr const test_ref;

/* **** */

#define Kb(_x) ((_x) * 1024)
#define Mb(_x) Kb(Kb(_x))

/* **** */

#include "libarmcc/include/armcc.hpp"

extern "C" {
	#include "libarmvm/include/armvm.h"
}

/* **** */

#define kTEST_MEM_ALLOC Kb(64)

typedef struct test_tag {
	armcc_p cc;
	armvm_ptr armvm;
//
	uint32_t test_pc;
}test_t;

class test {
	protected:
		armcc cc;
		test_ptr t;
//
	public:
		int check_nz(const int n, const int z);
		int check_nzc(const int n, const int z, const int c);
		int check_nzcv(const int n, const int z, const int c, const int v);
		void reset(void);
		uint32_t run_test(void);
		uint32_t run_test_flags(const uint32_t flag_set);
		uint32_t step_test(void);
		test();
		~test();
//
	private:
		uint8_t mem[kTEST_MEM_ALLOC];
		test_t tt;
};
