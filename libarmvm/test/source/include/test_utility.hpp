#pragma once

/* **** */

#define pARMVM t->armvm
#define pCORE pARMVM->core
#define pMEM pARMVM->mem

extern "C" {
//	#include "libarmvm/armvm/armvm_glue.h"
	//#include "libarmvm/include/armvm_mem.h"
	#include "libarmvm/source/armvm.h"
	#include "libarmvm/include/libarmvm_test.h"

	#include "libarm/include/cc.h"
	#include "libarm/include/apsr/apsr.h"
	#include "libarm/include/dp.h"
	#include "libarm/include/sop.h"
	#include "libarm/include/strings.h"
}

/* **** */

#define fail_if(_x) ({if(_x) { LOG(">> FAILED... in \"%s\" on line: %d", __func__, __LINE__); }})

#define LOG_R(_x) LOG("%s: 0x%08x", arm_reg_name_lcase_string[0][_x], vR(_x))

/* **** */

#define GEN_COUNT t->cc->icount

#define GEN_IP t->cc->ip
#define GEN_PC t->cc->pc

#define TEST_PC t->test_pc

#define TRACE_APSR(psr) \
	do { \
		LOG(#psr " -- N = %1u, Z = %1u, C = %1u, V = %1u -- 0x%08x", \
			ARM_APSR_BEXT(psr, N), ARM_APSR_BEXT(psr, Z), \
			ARM_APSR_BEXT(psr, C), ARM_APSR_BEXT(psr, V), psr); \
	}while(0);
