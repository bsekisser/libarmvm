#pragma once

/* **** */

typedef enum arm_dp_opcode_enum {
	ARM_AND,
	ARM_EOR,
	ARM_SUB,
	ARM_RSB,
//
	ARM_ADD,
	ARM_ADC,
	ARM_SBC,
	ARM_RSC,
//
	ARM_TST,
	ARM_TEQ,
	ARM_CMP,
	ARM_CMN,
//
	ARM_ORR,
	ARM_MOV,
	ARM_BIC,
	ARM_MVN,
}arm_dp_opcode_enum;
typedef arm_dp_opcode_enum const arm_dp_opcode_eref;

typedef enum thumb_dp_opcode_enum {
// TUUMB EXTENSIONS
	ARM_ASR = 16,
	ARM_LSL,
	ARM_LSR,
	ARM_ROR,
//
	ARM_MUL,
	ARM_NEG, THUMB_NEG = ARM_NEG,
//
}thumb_dp_opcode_enum;
typedef thumb_dp_opcode_enum const thumb_dp_opcode_eref;
