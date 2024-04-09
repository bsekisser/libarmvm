#include "armvm_trace_reg.h"

/* **** */

#include "armvm_utility.h"

/* **** */

void arm_reg_dst(armvm_p avm, unsigned rrx, unsigned rdst)
{
	LOG("rrx: 0x%02x, rsrc: 0x%02x", rrs, rdst);

	if(pTRACE)
		vRx(rrx) = GPRx(rdst);
}

/*void arm_reg_dst_wb(armvm_p avm, unsigned rrx, unsigned rsrc, uint32_t write)
{
	GPRx(rdst) = write;

	if(pTRACE)
		vRx(rrx) = write;
}*/

uint32_t arm_reg_src(armvm_p avm, unsigned rrx, unsigned rsrc)
{
	const uint32_t vr = GPRx(rsrc);

	LOG("rrx: 0x%02x, rsrc: 0x%02x, vr: 0x%08x", rrs, rsrc, vr);

	if(ARMVM_GPR(PC) == rsrc)
		vr += 4;

	LOG("rrx: 0x%02x, rsrc: 0x%02x, vr: 0x%08x", rrs, rsrc, vr);

	if(pTRACE)
		vRx(rrx) = vr;

	return(vr);
}

/*void arm_reg64p_dst(armvm_p avm, unsigned rrx, uint64_t* write)
{
	if(pTRACE)
		vR64x(rrx) = *write;
}*/

uint32_t setup_rR_vR(armvm_p avm, unsigned rrx, unsigned rr, uint32_t vr)
{
	LOG("rrx: 0x%02x, rr: 0x%02x, vr: 0x%08x", rr, vr);

	if(pTRACE) {
		rRx(rrx) = rr;
		vRx(rrx) = vr;
	}

	return(vr);
}
