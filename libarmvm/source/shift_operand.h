/* **** */

#include "data_processing.h"

/* **** */


typedef struct shift_operand_tref {
	union {
		struct {
			unsigned char amount:4;
			unsigned char immediate;
		}rotate;
		struct {
			unsigned char i:1;
			union {
				unsigned char rs:4;
				unsigned char amount:5;
			};
			unsigned char rm:4;
			unsigned char type:2;
		}shift;
	};
	unsigned char co:1;
}shift_operand_t;

static
uint32_t shift_operand_dp(shift_operand_tref so, armvm_ref avm)
{
	const uint32_t rm = dp.i
		? dp.rotate.immediate
		: reg_src(avm, dp.shift.rm);

	const uint32_t rn = is_mov ? 0 : reg_src(avm, dp.rn);

	const uint32_t rs = dp.i
		? dp.rotate.amount
		: dp.shift.i
			? dp.shift.amount
			: reg_src(avm, dp.shift.rs);

	const uint32_t sop = dp.i
		? shiftbox(&dp, avm, ARM_ROR, rm, rs);
		: (dp.shift.i ? shiftbox_immediate : shiftbox)
			(&dp, avm, dp.shift.type, rm, rs);
