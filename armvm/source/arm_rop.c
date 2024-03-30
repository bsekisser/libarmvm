static _arm_rop_b_bl_blx(int link, int x, rop_p rop, arm_rop_p arp)
{
	if(link) {
		CYCLE++;
		LR = PC;
	}

	CYCLE++;
	PC += rop.b.offset;

	arm_trace(pARM_TRACE, "b%s%s(0x%08x)",
		link ? "l" : "",
		x ? "x" : "",
		PC);
}

static void arm_rop__b(rop_p rop, arm_rop_p arp)
{ return(_arm_rop_b_bl(0, rop, arp)); }

static void arm_rop__bl(rop_p rop, arm_rop_p arp)
{ return(_arm_rop_b_bl(1, rop, arp)); }

/* **** */

int arm_rop_step(arm_rop_p arp);
