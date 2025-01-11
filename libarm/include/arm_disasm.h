#pragma once

/* **** */

#include <stdint.h>

/* **** */

void arm_disasm(uint32_t address, uint32_t opcode);
void arm_disasm_p(void* p, uint32_t address);

void arm_disasm_arm(uint32_t address, uint32_t opcode);
void arm_disasm_arm_p(void* p, uint32_t address);

void arm_disasm_thumb(uint32_t address, uint32_t opcode);
void arm_disasm_thumb_p(void* p, uint32_t address);
