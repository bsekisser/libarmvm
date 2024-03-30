#pragma once

/* **** */

typedef struct armvm_config_t** armvm_config_h;
typedef struct armvm_config_t* armvm_config_p;

/* **** */

enum {
	arm_v4,
	arm_v4t = 1,
	arm_v5t = 1,
	arm_v5te = 2,
	arm_v5texp = 2,
	arm_v5tej,
	arm_v6,
};

typedef struct armvm_config_t {
	struct {
		unsigned thumb:1;
	}features;
	struct {
		unsigned ir_checks:1;
	}pedantic;
	struct {
		unsigned alloc:1;
		unsigned alloc_init:1;
		unsigned exit:1;
		unsigned init:1;
	}trace;
	unsigned version;
}armvm_config_t;
