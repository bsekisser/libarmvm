#pragma once

/* **** */

typedef struct armvm_config_t** armvm_config_h;
typedef struct armvm_config_t* armvm_config_p;

/* **** */

#include "armvm_core_config.h"
#include "armvm_mem_config.h"

/* **** */

typedef struct armvm_config_t {
	armvm_core_config_p core;
	armvm_mem_config_p mem;
	struct {
		unsigned alloc:1;
		unsigned alloc_init:1;
		unsigned exit:1;
		unsigned init:1;
	}trace;
}armvm_config_t;
