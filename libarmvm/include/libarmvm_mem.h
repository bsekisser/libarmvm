#pragma once

/* **** */

#include "libarmvm.h"

/* **** */

#include <stdint.h>
#include <stddef.h>

/* **** */

// TODO: should this be exported???
typedef uint32_t (*libarmvm_mem_fn)(void *const param, const uint32_t ppa, const size_t size, uint32_t *const write);

/* **** */

void libarmvm_mem_mmap_cb(libarmvm_ref avm,
	const uint32_t base, const uint32_t end,
	libarmvm_mem_fn const fn, void *const param);

void libarmvm_mem_mmap_ro(libarmvm_ref avm,
	const uint32_t base, const uint32_t end, void *const data);

void libarmvm_mem_mmap_rw(libarmvm_ref avm,
	const uint32_t base, const uint32_t end, void *const data);
