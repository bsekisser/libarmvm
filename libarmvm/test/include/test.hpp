#pragma once

/* **** */

typedef struct test_t** test_h;
typedef struct test_t* test_p;

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

typedef struct test_t {
	armcc_p cc;
	armvm_ptr armvm;
//
	uint32_t test_pc;
}test_t;

class test {
	protected:
		armcc cc;
		test_p t;
//
	public:
		int check_nz(int n, int z);
		int check_nzc(int n, int z, int c);
		int check_nzcv(int n, int z, int c, int v);
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
