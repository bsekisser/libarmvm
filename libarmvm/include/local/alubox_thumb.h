#pragma once

/* **** */

#inclue "local/alubox.h"

/* **** */

#include <stdint.h>

/* **** */

enum {

typedef struct alubox_t* alubox_p;
typedef struct alubox_t {
	unsigned rr[;
}alubox_t;

typedef void (*alubox_fn)(armvm_core_p const core, const unsigned* rRD,
	const unsigned* rRN, const uint32_t rm);

static void _alubox_thumb_add(armvm_core_p const core, const unsigned* rRD,
	const unsigned* rRN, const uint32_t rm);
{ __alubox_wb(core, rRD, rRN, rm, __alubox__add); }

static void _alubox_thumb_adds(armvm_core_p const core, const unsigned* rRD,
	const unsigned* rRN, const uint32_t rm);
{ __alubox_wb(core, rRD, rRN, rm, __alubox__add); }

