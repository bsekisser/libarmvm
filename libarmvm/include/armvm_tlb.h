#pragma once

/* **** */

typedef struct armvm_tlb_t** armvm_tlb_h;
typedef struct armvm_tlb_t* armvm_tlb_p;

typedef struct armvm_tlbe_t** armvm_tlbe_h;
typedef struct armvm_tlbe_t* armvm_tlbe_p;

#include "armvm_mmu.h"
#include "armvm.h"

/* **** */

void armvm_tlb(armvm_tlb_p const tlb, const unsigned action);
armvm_tlb_p armvm_tlb_alloc(armvm_p const avm, armvm_mmu_p const mmu, armvm_tlb_h const h2tlb);
void armvm_tlb_fill_data_tlbe(armvm_tlbe_p const tlbe, const uint32_t va, armvm_mem_callback_p const cb);
void armvm_tlb_fill_instruction_tlbe(armvm_tlbe_p const tlbe, const uint32_t va, armvm_mem_callback_p const cb);
armvm_mem_callback_p armvm_tlb_ifetch(armvm_tlb_p const tlb, const uint32_t va, armvm_tlbe_h const h2tlbe);
armvm_mem_callback_p armvm_tlb_read(armvm_tlb_p const tlb, const uint32_t va, armvm_tlbe_h const h2tlbe);
armvm_mem_callback_p armvm_tlb_write(armvm_tlb_p const tlb, const uint32_t va, armvm_tlbe_h const h2tlbe);
