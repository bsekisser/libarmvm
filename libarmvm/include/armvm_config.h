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
}armvm_config_t;
