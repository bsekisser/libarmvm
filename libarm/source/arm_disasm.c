#include "arm_disasm.h"

/* **** */

#include "git/libbse/include/bitfield.h"
#include "git/libbse/include/capstone_assert_test.h"
#include "git/libbse/include/log.h"
#include "git/libbse/include/unused.h"

/* **** */

#include <capstone/capstone.h>

/* **** */

static void _arm_disasm_(uint32_t address, uint32_t opcode, int thumb)
{
	csh handle = 0;
	cs_insn *insn;

	const size_t size = thumb ? sizeof(uint16_t) : sizeof(uint32_t);
	const int mode = thumb ? CS_MODE_THUMB : CS_MODE_ARM;

	cs_assert_success(cs_open(CS_ARCH_ARM, mode, &handle));

	const uint8_t* insn_data = (uint8_t*)&opcode;
	uint64_t insn_addr = address;

	size_t count = cs_disasm(handle, insn_data, size, insn_addr, 0, &insn);

	if (count > 0) {
		for(unsigned j = 0; j < count; j++) {
			const cs_insn* insn_j = &insn[j];

			const uint64_t insn_address = insn_j->address;
			printf("0x%08" PRIx64 ":\t", insn_address);
			for(unsigned int k = 0; k < size; k++)
				printf(" 0x%02x", insn_data[(j << 2) + k]);
			printf("\t\t%s\t\t%s\n", insn_j->mnemonic,
					insn_j->op_str);
		}
	} else
		printf("0x%02zx:[0x%08x] == 0x%08x: Failed to disassemble given code!\n",
			size, address, opcode);

	cs_close(&handle);
}

static void _arm_disasm_iter(uint32_t address, uint32_t opcode, int thumb)
{
	csh handle = 0;
	cs_insn insn;

	size_t size = thumb ? sizeof(uint16_t) : sizeof(uint32_t);
	const int mode = thumb ? CS_MODE_THUMB : CS_MODE_ARM;

	cs_assert_success(cs_open(CS_ARCH_ARM, mode, &handle));

	const uint8_t* insn_data = (uint8_t*)&opcode;
	uint64_t insn_addr = address;

	size_t count = cs_disasm_iter(handle, &insn_data, &size, &insn_addr, &insn);

	if (count > 0) {
		const uint64_t insn_address = insn.address;
		printf("0x%08" PRIx64 ":\t", insn_address);
		for(unsigned int k = 0; k < size; k++)
			printf(" 0x%02x", insn_data[k]);
		printf("\t\t%s\t\t%s\n", insn.mnemonic,
				insn.op_str);
	} else
		printf("0x%08x:(0x%02zx): Failed to disassemble given code!\n", address, size);

	cs_close(&handle);
}

static void _arm_disasm(uint32_t address, uint32_t opcode, int thumb)
{
	if(0)
		return(_arm_disasm_iter(address, opcode, thumb));

	return(_arm_disasm_(address, opcode, thumb));
}

void arm_disasm(uint32_t address, uint32_t opcode)
{
	if(address & 1)
		return(arm_disasm_thumb(address, opcode));

	return(arm_disasm_arm(address, opcode));
}

void arm_disasm_arm(uint32_t address, uint32_t opcode)
{
	return(_arm_disasm(address & ~3, opcode, 0));
}

void arm_disasm_thumb(uint32_t address, uint32_t opcode)
{
	return(_arm_disasm(address & ~1, opcode, 1));
}
