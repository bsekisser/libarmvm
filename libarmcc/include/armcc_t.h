#pragma once

/* **** */

typedef struct armcc_t** armcc_h;
typedef struct armcc_t* armcc_p;

/* **** */

#include <stdint.h>

/* **** */

typedef struct armcc_t {
	uint32_t cs, ip, pc;
	uint32_t ds, dp;
//
	void *p2data, *p2cs, *p2ds;
}armcc_t;
