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

#include "libbse/include/bitops.h"

/* **** */

#define dTLB_BITS 8
#define iTLB_BITS 8

typedef struct armvm_tlbe_tag {
	armvm_mem_callback_ptr cb;
//
	struct {
		uint32_t i:1;
		uint32_t vp:20;
	};
}armvm_tlbe_t;

typedef struct armvm_tlb_tag {
	armvm_tlbe_t dtlb[_BV(dTLB_BITS)];
	armvm_tlbe_t itlb[_BV(iTLB_BITS)];
//
	armvm_ptr armvm;
	armvm_mmu_ptr mmu;
}armvm_tlb_t;

/* **** */

#include "libbse/include/action.h"

/* **** */

void armvm_tlb_cp15_init(armvm_tlb_ref tlb);

void armvm_tlb(armvm_tlb_ref tlb, action_ref action);
armvm_tlb_ptr armvm_tlb_alloc(armvm_ref avm, armvm_mmu_ref mmu, armvm_tlb_href h2tlb);
void armvm_tlb_fill_data_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_ref cb);
void armvm_tlb_fill_instruction_tlbe(armvm_tlbe_ref tlbe, const uint32_t va, armvm_mem_callback_ref cb);
armvm_mem_callback_ptr armvm_tlb_ifetch(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
void armvm_tlb_invalidate_all(armvm_tlb_ref tlb);
void armvm_tlb_invalidate_data(armvm_tlb_ref tlb);
void armvm_tlb_invalidate_instruction(armvm_tlb_ref tlb);
armvm_mem_callback_ptr armvm_tlb_read(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
armvm_mem_callback_ptr armvm_tlb_write(armvm_tlb_ref tlb, const uint32_t va, armvm_tlbe_href h2tlbe);
