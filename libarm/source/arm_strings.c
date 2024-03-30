#include "arm_strings.h"

/* **** */

#include "arm_sop.h"

/* **** */

const char* arm_cc_string[2][16] = {
	{ "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
	  "hi", "ls", "ge", "lt", "gt", "le",   "",   "" },
	{ "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
	  "hi", "ls", "ge", "lt", "gt", "le", "al", "nv" },
};

const char* arm_cc_ucase_string[2][16] = {
	{ "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
	  "HI", "LS", "GE", "LT", "GT", "LE",   "",   "" },
	{ "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
	  "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV" },
};

const char* arm_creg_name_string[16] = {
	"c0",  "c1",  "c2",  "c3",  "c4",  "c5",  "c6",  "c7",
	"c8",  "c9", "c10", "c11", "c12", "c13", "c14", "c15",
};

const char* arm_dp_inst_string[16] = {
	"and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc",
	"tst", "teq", "cmp", "cmn", "orr", "mov", "bic", "mvn",
};

const char* arm_dp_op_string[16] = {
	"& ", "^ ", "- ", "- ", "+ ", "+ ",  "- ", "- ",
	"& ", "^ ", "- ", "+ ", "| ",  "=", "& ~",  "~"
};

const char* arm_reg_name_lcase_string[2][16] = {
	{  "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
	   "r8",  "r9", "r10", "r11", "r12", "rSP", "rLR", "rPC", },
	{ " r0", " r1", " r2", " r3", " r4", " r5", " r6", " r7",
	  " r8", " r9", "r10", "r11", "r12", "rSP", "rLR", "rPC", },
};

const char* arm_sop_lcase_string[_ARM_SOP_COUNT_] = {
	"lsl", "lsr", "asr", "ror", "rrx",
};

const char* arm_sop_ucase_string[_ARM_SOP_COUNT_] = {
	"LSL", "LSR", "ASR", "ROR", "RRX",
};
