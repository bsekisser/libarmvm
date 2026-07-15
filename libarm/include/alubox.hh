#pragma once

/* **** */

#include "apsr.hh"
#include "reg.hh"

#include "libbse/include/shift_roll_32.h"

/* **** */

class alubox_class {
public:
	alubox_class(arm_apsr_ref apsr, arm_reg_ref reg):apsr(apsr),reg(reg) {}
//
	uint32_t ADC(arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.adc(reg.src(rN, rrn), sop)); }
	uint32_t ADC(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, ADC(rrn, sop))); }
	uint32_t ADCS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.adcs(reg.src(rN, rrn), sop))); }

//
	uint32_t ADD(arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.src(rN, rrn) + sop); }
	uint32_t ADD(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, ADD(rrn, sop))); }
	uint32_t ADDS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.adds(reg.src(rN, rrn), sop))); }
//
	uint32_t AND(arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.src(rN, rrn) & sop); }
	uint32_t AND(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, AND(rrn, sop))); }
	uint32_t ANDS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(AND(rrd, rrn, sop))); }
//
	uint32_t ASR(arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.asr(reg.src(rM, rrm), rs ?: 32)); }
	uint32_t ASR(arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.asr(reg.src(rM, rrm), reg.src(rS, rrs))); }
	uint32_t ASR(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(reg.dst_wb_v(rD, rrd, ASR(rrm, rs))); }
	uint32_t ASR(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(reg.dst_wb_v(rD, rrd, ASR(rrm, rrs))); }
	uint32_t ASRS(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.nzc(ASR(rrd, rrm, rs))); }
	uint32_t ASRS(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.nzc(ASR(rrd, rrm, rrs))); }
//
	uint32_t BIC(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, reg.src(rN, rrn) & ~sop)); }
	uint32_t BICS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(BIC(rrd, rrn, sop))); }
//
	uint32_t CMNS(arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.adds(reg.src(rN, rrn), sop)); }
//
	uint32_t CMPS(arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.subs(reg.src(rN, rrn), sop)); }
//
	uint32_t EOR(arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.src(rN, rrn) ^ sop); }
	uint32_t EOR(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, EOR(rrn, sop))); }
	uint32_t EORS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(EOR(rrd, rrn, sop))); }
//
	uint32_t LSL(arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.lsl(reg.src(rM, rrm), rs)); }
	uint32_t LSL(arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(LSL(rrm, reg.src(rS, rrs))); }
	uint32_t LSL(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(reg.dst_wb_v(rD, rrd, LSL(rrm, rs))); }
	uint32_t LSL(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(reg.dst_wb_v(rD, rrd, LSL(rrm, rrs))); }
	uint32_t LSLS(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.nzc(LSL(rrd, rrm, rs))); }
	uint32_t LSLS(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.nzc(LSL(rrd, rrm, rrs))); }
//
	uint32_t LSR(arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.lsr(reg.src(rM, rrm), rs ?: 32)); }
	uint32_t LSR(arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.lsr(reg.src(rM, rrm), reg.src(rS, rrs))); }
	uint32_t LSR(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(reg.dst_wb_v(rD, rrd, LSR(rrm, rs))); }
	uint32_t LSR(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(reg.dst_wb_v(rD, rrd, LSR(rrm, rrs))); }
	uint32_t LSRS(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.nzc(LSR(rrd, rrm, rs))); }
	uint32_t LSRS(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.nzc(LSR(rrd, rrm, rrs))); }
//
	uint32_t MOV(arm_gpr_eref rrd, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, sop)); }
	uint32_t MOVS(arm_gpr_eref rrd, const uint32_t sop)
		{ return(apsr.nzc(MOV(rrd, sop))); }
//
	uint32_t MVN(arm_gpr_eref rrd, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, ~sop)); }
	uint32_t MVNS(arm_gpr_eref rrd, const uint32_t sop)
		{ return(apsr.nzc(MVN(rrd, sop))); }
//
	uint32_t ORR(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, reg.src(rN, rrn) | sop)); }

	uint32_t ORRS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(ORR(rrd, rrn, sop))); }
//
	uint32_t ROR(arm_gpr_eref rrm, const uint32_t rs)
		{
			const uint32_t rm = reg.src(rM, rrm);
			return(rs ? apsr.ror(rm, rs) : apsr.rrx(rm));
		}
	uint32_t ROR(arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.ror(reg.src(rM, rrm), reg.src(rS, rrs))); }
	uint32_t ROR(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(reg.dst_wb_v(rD, rrd, ROR(rrm, rs))); }
	uint32_t ROR(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(reg.dst_wb_v(rD, rrd, ROR(rrm, rrs))); }
	uint32_t RORS(arm_gpr_eref rrd, arm_gpr_eref rrm, const uint32_t rs)
		{ return(apsr.nzc(ROR(rrd, rrm, rs))); }
	uint32_t RORS(arm_gpr_eref rrd, arm_gpr_eref rrm, arm_gpr_eref rrs)
		{ return(apsr.nzc(ROR(rrd, rrm, rrs))); }

//
	uint32_t RSB(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, sop - reg.src(rN, rrn))); }
	uint32_t RSBS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.subs(sop, reg.src(rN, rrn)))); }
//
	uint32_t RSC(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.sbc(sop, reg.src(rN, rrn)))); }
	uint32_t RSCS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.sbcs(sop, reg.src(rN, rrn)))); }
//
	uint32_t SBC(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.sbc(reg.src(rN, rrn), sop))); }
	uint32_t SBCS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.sbcs(reg.src(rN, rrn), sop))); }
//
	uint32_t SUB(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, reg.src(rN, rrn) - sop)); }
	uint32_t SUBS(arm_gpr_eref rrd, arm_gpr_eref rrn, const uint32_t sop)
		{ return(reg.dst_wb_v(rD, rrd, apsr.subs(reg.src(rN, rrn), sop))); }
//
	uint32_t TEQS(arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(EOR(rrn, sop))); }
//
	uint32_t TSTS(arm_gpr_eref rrn, const uint32_t sop)
		{ return(apsr.nzc(AND(rrn, sop))); }

private:
	arm_apsr_ref apsr;
	arm_reg_ref reg;
};
