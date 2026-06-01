#pragma once

/* **** */

typedef struct libarmvm_tag** libarmvm_hptr;
typedef libarmvm_hptr const libarmvm_href;

typedef struct libarmvm_tag* libarmvm_ptr;
typedef libarmvm_ptr const libarmvm_ref;

#include "libarmvm/include/libarmvm_state.h"

/* **** */

#include "git/libbse/include/action.h"

/* **** */

#define PUBLIC __attribute__((visibility("default")))
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

/* **** */

extern action_list_t libarmvm_action_list;

void libarmvm(libarmvm_ref avm, action_ref action);

void libarmvm_alloc_init(libarmvm_ref avm);

WARN_UNUSED_RESULT
libarmvm_ptr libarmvm_alloc(void);

WARN_UNUSED_RESULT
uint64_t libarmvm_cycle(libarmvm_ref avm);

void libarmvm_exit(libarmvm_ref avm);

libarmvm_ptr libarmvm_halloc(libarmvm_href h2avm);

WARN_UNUSED_RESULT
uint64_t libarmvm_icount(libarmvm_ref avm);

WARN_UNUSED_RESULT
uint32_t libarmvm_ip(libarmvm_ref avm);

WARN_UNUSED_RESULT
uint32_t libarmvm_pc(libarmvm_ref avm);

void libarmvm_reset(libarmvm_ref avm);

//__attribute__((warn_unused_result))
uint64_t libarmvm_run(libarmvm_ref avm, uint64_t cycles);

// WARN_UNUSED_RESULT
libarmvm_state_t libarmvm_step(libarmvm_ref avm);

void* libarmvm_threaded_run(void* param);

// WARN_UNUSED_RESULT
int libarmvm_threaded_start(libarmvm_ref avm);

unsigned libarmvm_trace(libarmvm_ref avm, int trace_set);
