#pragma once

/* **** */

typedef struct armvm_core_config_t* armvm_core_config_p;

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

typedef struct armvm_core_config_t {
	struct {
		unsigned thumb:1;
	}features;
	struct {
		unsigned ir_checks:1;
	}pedantic;
	unsigned trace:1;
	unsigned version;
}armvm_core_config_t;
