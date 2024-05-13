extern "C" {
	#include "test_utility.h"
	#include "test.h"
}

/* **** */

#include "test_arm_gen.hpp"

/* **** */

extern "C" {
	int test_arm_add(test_p t)
{
	return(0);
	reset(t);

	rR(0) = 0;
	add(r0, r0, 1);
	run_test(t);
	fail_if(1 != rR(0));

	add(r0, r0, LSL(r0, 1));
	run_test(t);
	fail_if(3 != rR(0));

	add(r0, rPC, 0);
	run_test(t);
	fail_if(8 != (rR(0) - TEST_PC));

	return(1);
}

static void _s_adds_in(test_p const t, const int32_t rd, const int32_t rn,
	const unsigned v)
	{
		rR(0) = rd; rR(1) = rn;
		adds(r0, r0, r1);

		run_test_flags(t, v ? _BV(ARM_CPSR(V)) : 0);
	}

static void _u_adds_in(test_p const t, const uint32_t rd, const uint32_t rn,
	const unsigned c)
	{
		rR(0) = rd; rR(1) = rn;
		adds(r0, r0, r1);

		run_test_flags(t, c ? _BV(ARM_CPSR(C)) : 0);
	}

static void _out_zc(test_p const t, const uint32_t rd,
	const unsigned z, const unsigned c)
	{
		fail_if(rd != rR(0));
		check_nzcv(t, IF_CPSR(N), z, c, IF_CPSR(V));
	}

static void _out_nzc(test_p const t, const uint32_t rd,
	const unsigned n, const unsigned z, const unsigned c)
	{
		fail_if(rd != rR(0));
		check_nzcv(t, n, z, c, IF_CPSR(V));
	}

static void _out_nzcv(test_p const t, const uint32_t rd,
	const unsigned n, const unsigned z, const unsigned c, const unsigned v)
	{
		fail_if(rd != rR(0));
		check_nzcv(t, n, z, c, v);
	}

	int test_arm_adds(test_p t)
{
	reset(t);

if(0) {
	rR(0) = 1;
	adds(r0, r0, LSL(r0, 3));
	run_test(t);
	fail_if(0x00000009 != rR(0));
	check_nzcv(t, 0, 0, 0, 0);

	rR(1) = 0x7fffffff; rR(2) = 0x7fffffff;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0xfffffffe != rR(0));
	check_nzcv(t, 1, 0, 0, 1);

	rR(1) = 0xffffffff; rR(2) = 0x00000001;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x00000000 != rR(0));
	check_nzcv(t, 0, 1, 1, 0);

	rR(1) = 0x60000000; rR(2) = 0x20000000;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x80000000 != rR(0));
	check_nzcv(t, 1, 0, 0, 1);

	rR(1) = 0x60000000; rR(2) = 0xc0000000;
	adds(r0, r1, r2);
	run_test(t);
	fail_if(0x20000000 != rR(0));
	check_nzcv(t, 0, 0, 1, 0);
}

	_u_adds_in(t, 0xfffffffe, 0x00000001, 0);
	_out_nzcv(t, 0xffffffff, 1, 0, 0, 0);

	_u_adds_in(t, 0xfffffffe, 0x00000002, 0);
	_out_nzcv(t, 0x00000000, 0, 1, 1, 0);

	_u_adds_in(t, 0xfffffffe, 0x00000003, 0);
	_out_nzcv(t, 0x00000001, 0, 0, 1, 0);

	_s_adds_in(t, 0x7ffffffe, 0x00000001, 0);
	_out_nzcv(t, 0x7fffffff, 0, 0, 0, 0);

	_s_adds_in(t, 0x7ffffffe, 0x00000002, 0);
	_out_nzcv(t, 0x80000000, 1, 0, 0, 1);

	_s_adds_in(t, 0x80000001, 0xffffffff, 0);
	_out_nzcv(t, 0x80000000, 1, 0, 1, 0);

	_s_adds_in(t, 0x80000001, 0xfffffffe, 0);
	_out_nzcv(t, 0x7fffffff, 0, 0, 1, 1);

	return(1);
}}
