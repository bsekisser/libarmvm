static int32_t _gen_arm__pc_byte_offset(armcc_p const p2cc, const uint32_t pat)
{ return(-8 + (pat - (cc.cs + cs.pc))); }

static int32_t _gen_arm__pc_word_offset(armcc_p const p2cc, const uint32_t pat)
{ return(_gen_arm__pc_byte_offset(p2cc, pat) >> 2); }

namespace arm {
	namespace b {
		static uint32_t offset(armcc_p p2cc, const uint32_t pat)
		{
			const uint32_t cs_pc = p2cc->cs + p2cc->pc;

			if(0) LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);

			const uint32_t offset = _gen_arm__pc_word_offset(p2cc, pat);
			const uint32_t offset_masked = offset & pbBF(0, 24);

			if(0) {
				LOG_START("offset: 0x%08x", offset);
				LOG_END(", offset_masked: 0x%08x", offset_masked);
			}

			assert(offset == pbBFEXTs(offset_masked, 0, 24));

			return(offset_masked);
		}
	}

	namespace ldst {
		static uint32_t offset(armcc_p p2cc,
			const unsigned pat, unsigned *const bit_u)
		{
			assert(p2cc);
			assert(bit_u);

			const uint32_t cs_pc = p2cc->cs + p2cc->pc;

			if(0) {
				LOG("(cs: 0x%08x, pc: 0x%08x): 0x%08x", p2cc->cs, p2cc->pc, cs_pc);
			}

			const uint32_t offset = _gen_arm__pc_byte_offset(p2cc, pat);
			const uint32_t offset_masked = offset & pbBF(0, 12);

			if(0) {
				LOG_START("offset: 0x%08x", offset);
				LOG_END(", offset_masked: 0x%08x", offset_masked);
			}

		//	assert(offset == pbBFEXTs(offset_masked, 0, 12));

			*bit_u = !((int32_t)offset < 0);

			return(offset_masked);
		}
	}
}
