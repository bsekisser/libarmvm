#pragma once

/* **** */

typedef struct test_t** test_h;
typedef struct test_t* test_p;

/* **** */

#define Kb(_x) ((_x) * 1024)
#define Mb(_x) Kb(Kb(_x))

/* **** */

#include "libarmvm/include/armvm.h"

/* **** */

typedef struct test_t {
	armvm_p armvm;
	struct {
		unsigned count;
		uint32_t ip;
		uint32_t pc;
	}gen;
	uint32_t test_pc;
//
	uint8_t mem[Mb(1)];
}test_t;

#define pARMVM t->armvm
#define pCORE pARMVM->core

int check_nz(test_p t, int n, int z);
int check_nzc(test_p t, int n, int z, int c);
int check_nzcv(test_p t, int n, int z, int c, int v);
void reset(test_p t);
uint32_t run_test(test_p t);
