#pragma once

/* **** */

typedef struct armvm_mem_config_t* armvm_mem_config_p;

/* **** */

typedef struct armvm_mem_config_t {
	unsigned trace_mmap:1;
	struct {
		unsigned mmap_alloc;
		struct {
			struct {
				unsigned free:1;
				unsigned malloc:1;
			}alloc;
		}mmap;
	}trace;
}armvm_mem_config_t;
