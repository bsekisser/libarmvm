#include "arm_disasm.h"

/* **** */

#include "libbse/include/bitfield.h"
#include "libbse/include/capstone_assert_test.h"
#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <capstone/capstone.h>

/* **** */

static void _arm_disasm(uint8_t* p, uint32_t address, int thumb)
{
	csh handle = 0;
	cs_insn *insn;

	const size_t size = thumb ? sizeof(uint16_t) : sizeof(uint32_t);
	const int mode = thumb ? CS_MODE_THUMB : CS_MODE_ARM;

	cs_assert_success(cs_open(CS_ARCH_ARM, mode, &handle));

	uint64_t insn_addr = address;

	size_t count = cs_disasm(handle, p, size, insn_addr, 0, &insn);

	if (count > 0) {
		for(unsigned j = 0; j < count; j++) {
			const cs_insn* insn_j = &insn[j];

			const uint64_t insn_address = insn_j->address;
			printf("0x%08" PRIx64 ":\t", insn_address);
			for(unsigned int k = 0; k < size; k++)
				printf(" 0x%02x", p[(j << 2) + k]);
			printf("\t\t%s\t\t%s\n", insn_j->mnemonic,
					insn_j->op_str);
		}
		cs_free(insn, count);
	} else
		printf("0x%02zx:[0x%08x] == 0x%08x: Failed to disassemble given code!\n",
			size, address, htole32(*(uint32_t*)p));

	cs_close(&handle);
}

void arm_disasm(uint32_t address, uint32_t opcode)
{ return(_arm_disasm((void*)&opcode, address, address & 1)); }

void arm_disasm_p(void* p, uint32_t address)
{ return(_arm_disasm(p, address, address & 1)); }

void arm_disasm_arm(uint32_t address, uint32_t opcode)
{ return(_arm_disasm((void*)&opcode, address & ~3U, 0)); }

void arm_disasm_arm_p(void* p, uint32_t address)
{ return(_arm_disasm(p, address & ~3U, 0)); }

void arm_disasm_thumb(uint32_t address, uint32_t opcode)
{ return(_arm_disasm((void*)&opcode, address & ~1U, 1)); }

void arm_disasm_thumb_p(void* p, uint32_t address)
{ return(_arm_disasm(p, address & ~1U, 1)); }
