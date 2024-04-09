#pragma once

/* **** */

typedef struct armvm_tlb_t** armvm_tlb_h;
typedef struct armvm_tlb_t* armvm_tlb_p;

typedef struct armvm_tlbe_t** armvm_tlbe_h;
typedef struct armvm_tlbe_t* armvm_tlbe_p;

#include "armvm_mmu.h"
#include "armvm.h"

/* **** */

void armvm_tlb(unsigned const action, armvm_tlb_p const tlb);
armvm_tlb_p armvm_tlb_alloc(armvm_tlb_h const h2tlb, armvm_mmu_p const mmu, armvm_p const avm);
void armvm_tlb_fill_data_tlbe(const uint32_t va, armvm_mem_callback_p const cb, armvm_tlbe_p const tlbe);
void armvm_tlb_fill_instruction_tlbe(const uint32_t va, armvm_mem_callback_p const cb, armvm_tlbe_p const tlbe);
armvm_mem_callback_p armvm_tlb_ifetch(uint32_t const va, armvm_tlbe_h const h2tlbe, armvm_tlb_p const tlb);
armvm_mem_callback_p armvm_tlb_read(uint32_t const va, armvm_tlbe_h const h2tlbe, armvm_tlb_p const tlb);
armvm_mem_callback_p armvm_tlb_write(uint32_t const va, armvm_tlbe_h const h2tlbe, armvm_tlb_p const tlb);
