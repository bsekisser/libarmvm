#pragma once

/* **** */
/* **** */

#include <stddef.h>
#include <stdint.h>

/* **** */

static int armvm_core_mem_ifetch(uint32_t *const read, uint32_t ea, size_t size, armvm_core_p const core);
static int armvm_core_mem_read(uint32_t *const read, uint32_t ea, size_t size, armvm_core_p const core);
static int armvm_core_mem_write(uint32_t ea, size_t size, uint32_t const write, armvm_core_p const core);
