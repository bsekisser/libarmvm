#pragma once

/* **** */

#include "test.hpp"

/* **** */

class test_arm : public test {
	int adcs(void);
	int add(void);
	int adds(void);
	int ands(void);
	int b_bl(void);
	int bics(void);
	int mov(void);
	int movs(void);
	int rsbs(void);
	int subs(void);
	int shift(void);
//
	public:
		int run_tests(void);
};
