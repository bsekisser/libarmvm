#pragma once

/* **** */

#define pARMVM t->armvm
#define pARMVM_CORE pARMVM->core
#define pARMVM_MEM pARMVM->mem

extern "C" {
	#include "libarmvm/include/armvm_glue.h"
	//#include "libarmvm/include/armvm_mem.h"
	#include "libarmvm/include/armvm.h"

	#include "libarm/include/arm_cc.h"
	#include "libarm/include/arm_cpsr.h"
	#include "libarm/include/arm_dp.h"
	#include "libarm/include/arm_sop.h"
	#include "libarm/include/arm_strings.h"
}

/* **** */

#define fail_if(_x) ({if(_x) { LOG(">> FAILED... in \"%s\" on line: %d", __func__, __LINE__); }})

#define LOG_R(_x) LOG("%s: 0x%08x", arm_reg_name_lcase_string[0][_x], rR(_x))

/* **** */

#define rRx(_x) armvm_p2gpr(t->armvm, _x)[0]
#define rR(_x) rRx(r##_x)

#define GEN_COUNT t->cc->icount

#define GEN_IP t->cc->ip
#define GEN_PC t->cc->pc

#define RUN_PC rR(PC)

#define TEST_PC t->test_pc

#define TRACE_PSR(psr) \
	do { \
		LOG(#psr " -- N = %1u, Z = %1u, C = %1u, V = %1u -- 0x%08x", \
			ARM_CPSRx_BEXT(psr, N), ARM_CPSRx_BEXT(psr, Z), \
			ARM_CPSRx_BEXT(psr, C), ARM_CPSRx_BEXT(psr, V), psr); \
	}while(0);
