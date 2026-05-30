#pragma once

/* **** */

typedef enum arm_sop_enum {
	ARM_SOP_LSL,
	ARM_SOP_LSR,
	ARM_SOP_ASR,
	ARM_SOP_ROR,
	ARM_SOP_RRX,
//
	_ARM_SOP_COUNT_,
}arm_sop_enum;
typedef arm_sop_enum const arm_sop_eref;
