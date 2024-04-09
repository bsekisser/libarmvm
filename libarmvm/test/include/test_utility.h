#pragma once

/* **** */

#define pARMVM t->armvm
#define pARMVM_CORE pARMVM->core
//#define pARMVM_MEM pARMVM->mem

#include "libarmvm/include/armvm_glue.h"
//#include "libarmvm/include/armvm_mem.h"
#include "libarmvm/include/armvm.h"

#include "libarm/include/arm_cc.h"
#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_dp.h"
#include "libarm/include/arm_sop.h"
#include "libarm/include/arm_strings.h"

/* **** */

#define fail_if(_x) ({if(_x) { LOG(">> FAILED... in \"%s\" on line: %d", __func__, __LINE__); }})

#define LOG_R(_x) LOG("%s: 0x%08x", arm_reg_name_lcase_string[0][_x], rR(_x))

/* **** */

enum arm_reg_t {
	r0, r1,  r2,  r3,  r4,  r5,  r6,  r7,
	r8, r9, r10, r11, r12, r13, r14, r15,
//
	rSP = 13,
	rLR = r14,
	rPC = 15,
};

#define rRx(_x) pCORE->gpr[_x]
#define rR(_x) rRx(r##_x)

#define GEN_COUNT t->gen.count
#define GEN_IP t->gen.ip
#define GEN_PC t->gen.pc

#define RUN_PC rR(PC)

#define TEST_PC t->test_pc

#define TRACE_PSR(psr) \
	do { \
		LOG(#psr " -- N = %1u, Z = %1u, C = %1u, V = %1u -- 0x%08x", \
			ARM_CPSRx_BEXT(psr, N), ARM_CPSRx_BEXT(psr, Z), \
			ARM_CPSRx_BEXT(psr, C), ARM_CPSRx_BEXT(psr, V), psr); \
	}while(0);
