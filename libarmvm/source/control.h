#pragma once

/* **** */

#define _ARM_CONTROL_BSET(_bit) _ARM_CONTROL_BSET_AS(_bit, 1U)
#define _ARM_CONTROL_BSET_AS(_bit, _as) ((_as) << ARM_CONTROL_BIT(_bit))
#define ARM_CONTROL_BEXT(_v, _bit) (((_v) >> ARM_CONTROL_BIT(_bit)) & 1U)
#define ARM_CONTROL_BIT(_) CP15_REG1_BIT_##_
#define ARM_CONTROL_BSET(_v, _bit) ((_v) | _ARM_CONTROL_BSET(_bit))
#define ARM_CONTROL_BSET_AS(_v, _bit, _as) ((_v) | _ARM_CONTROL_BSET_AS(_bit, _as))
#define ARM_CONTROL_BSET_AS_WB(_v, _bit, _as) ((_v) |= _ARM_CONTROL_BSET_AS(_bit, _as))
#define ARM_CONTROL_BX(_data, _write, _lcase, _ucase) \
	({ \
		if(write) CONTROL(_lcase) = ARM_CONTROL_BEXT(data, _ucase); \
		else ARM_CONTROL_BSET_AS_WB(_data, _ucase, CONTROL(_lcase)); \
	})

typedef union control_tag* control_ptr;
typedef control_ptr const control_ref;

/* **** */

enum {
	CP15_REG1_BIT_M = 0,
	CP15_REG1_BIT_A = 1,
	CP15_REG1_BIT_V = 13,
	CP15_REG1_BIT_U = 20,
	CP15_REG1_BIT_EE = 25,
};

typedef union control_tag {
	unsigned raw_flags;
	struct {
		unsigned a:1; // 1
		unsigned ee:1; // 25
		unsigned m:1; // 0
		unsigned u:1; // 20
		unsigned v:1; // 13
	};
}control_t;
