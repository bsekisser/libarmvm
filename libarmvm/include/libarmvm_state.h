#pragma once

/* **** */

typedef union libarmvm_state_tag* libarmvm_state_ptr;
typedef libarmvm_state_ptr const libarmvm_state_ref;

#include "libarmvm.h"

/* **** */

typedef union libarmvm_state_tag {
	unsigned raw_flags;
	struct {
		struct {
			signed char error:1;
		};
		struct {
			struct {
				char data:1;
				char prefetch:1;
			}abort;
			char crashed:1;
			char halt:1;
			char high_vectors:1;
			char run:1;
			char running:1;
			char step:1;
			char threaded:1;
			char trace:1;
			char wfi:1;
		};
	};
}libarmvm_state_t;
typedef libarmvm_state_t const libarmvm_state_tref;

_Static_assert(4 == sizeof(libarmvm_state_t));

/* **** */

libarmvm_state_t libarmvm_state(libarmvm_ref avm);
