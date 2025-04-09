#pragma once

/* **** */

typedef struct armvm_mem_config_tag* armvm_mem_config_ptr;

/* **** */

typedef struct armvm_mem_config_tag {
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
