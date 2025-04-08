#pragma once

/* **** */

typedef struct armvm_tlb_tag** armvm_tlb_hptr;
typedef armvm_tlb_hptr const armvm_tlb_href;

typedef struct armvm_tlb_tag* armvm_tlb_ptr;
typedef armvm_tlb_ptr const armvm_tlb_ref;

typedef struct armvm_tlbe_tag** armvm_tlbe_hptr;
typedef armvm_tlbe_hptr const armvm_tlbe_href;

typedef struct armvm_tlbe_tag* armvm_tlbe_ptr;
typedef armvm_tlbe_ptr const armvm_tlbe_ref;

#include "armvm_mmu.h"
#include "armvm.h"

/* **** */

#include "libbse/include/action.h"

/* **** */

void armvm_tlb(armvm_tlb_ref tlb, action_ref action);
armvm_tlb_ptr armvm_tlb_alloc(armvm_p const avm, armvm_mmu_ref mmu, armvm_tlb_href h2tlb);
void armvm_tlb_fill_data_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_p const cb);
void armvm_tlb_fill_instruction_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_p const cb);
armvm_mem_callback_p armvm_tlb_ifetch(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
armvm_mem_callback_p armvm_tlb_read(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
armvm_mem_callback_p armvm_tlb_write(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
