#pragma once

/* **** */

typedef struct reg_t* reg_p;

/* **** */

#include "armvm_core.h"

#include "local/core_reg.h"

/* **** */

#include "libbse/include/unused.h"

/* **** */

#include <stdint.h>

/* **** */

typedef struct reg_t {
	uint32_t v;
//
	unsigned atx;
	unsigned r;
}reg_t;

/* **** */

static reg_p _core_reg_t_rR_vR(armvm_core_p const core, reg_p const reg,
	const unsigned atx, const unsigned r, const unsigned v)
{
	reg->atx = atx;
	reg->r = r;
	reg->v = v;

	setup_rR_vR(core, atx, r, v);
	return(reg);
}

/* **** */

static void core_reg_t_dst(armvm_core_p const core, reg_p const reg)
{ core_reg_dst_wb(core, reg->atx, reg->r, reg->v); }

static void core_reg_t_dst_wb(armvm_core_p const core, reg_p const reg,
	const uint32_t v)
{ core_reg_dst_wb(core, reg->atx, reg->r, (reg->v = v)); }

static reg_p core_reg_t_rR(armvm_core_p const core, reg_p const reg,
	const unsigned atx, const unsigned r)
{ return(_core_reg_t_rR_vR(core, reg, atx, r, 0)); }

static reg_p core_reg_t_vR(armvm_core_p const core, reg_p const reg,
	const unsigned atx, const uint32_t v)
{ return(_core_reg_t_rR_vR(core, reg, atx, ~0U, v)); }

static uint32_t core_reg_t_src(armvm_core_p core, reg_p reg)
{ return((reg->v = core_reg_src(core, reg->atx, reg->r))); }

/* **** */

#define tR_NAME(_x) rR_NAMEx(tR##_x.r)
