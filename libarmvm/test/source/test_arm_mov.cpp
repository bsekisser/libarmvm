#include "test_arm.hpp"
#include "test_utility.hpp"
#include "test.hpp"

/* **** */

#include "libarmcc/include/armcc.hpp"

/* **** */

int test_arm::mov(void)
{
	reset();

	cc.mov(rPC, rPC);
	run_test();
	fail_if(8 != (PC - TEST_PC));

	return(1);
}

int test_arm::movs(void)
{
	reset();

	cc.movs(rR0, 0);
	run_test();
	fail_if(0x00000000 != vR(0));
	check_nz(0, 1);

	vR(1) = ~0;
	cc.movs(rR0, rR1);
	run_test();
	fail_if(~0U != vR(0));
	check_nz(1, 0);

	cc.movs(rR0, cc.ror(64, 0));
	run_test();
	fail_if(0x00000040 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(64, 26));
	run_test();
	fail_if(0x00001000 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(64, 28));
	run_test();
	fail_if(0x00000400 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(0x3f, 0x1c));
	run_test();
	fail_if(0x000003f0 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(0xfc, 0x1e));
	run_test();
	fail_if(0x000003f0 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(0x80, 0));
	run_test();
	fail_if(0x00000080 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(0x80, 2));
	run_test();
	fail_if(0x00000020 != vR(0));
	check_nz(0, 0);

	cc.movs(rR0, cc.ror(0x80, 8));
	run_test();
	fail_if(0x80000000 != vR(0));
	check_nzc(1, 0, 1);

	return(1);
}
