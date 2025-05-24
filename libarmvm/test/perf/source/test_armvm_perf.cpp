//#include "test_armvm_perf.h"

/* **** */

#include "libarmcc/include/armcc.hpp"

extern "C" {
	#include "libarmvm/include/armvm.h"
}

extern "C" {
	#include "libbse/include/dtime.h"
	#include "libbse/include/log.h"
	#include "libbse/include/unused.h"
}

/* **** */

extern "C" {
	#include <inttypes.h>
	#include <stdint.h>
}

/* **** */

#define kTEST_MEM_ALLOC (1 << 16)
#define kTEST_MEM_END ((kTEST_MEM_ALLOC - 1) + kTEST_MEM_START)
#define kTEST_MEM_START 0

#define CINSTR ((1 * 1000) * 1000)

void test_armvm_perf__setup(void *const mem)
{
	armcc cc(mem, 0, 0x1000);

	cc.b(0x100);

	const uint32_t sp_dw = cc.dw(kTEST_MEM_ALLOC - 4);
	const uint32_t cinstr = cc.dw(CINSTR);
	const uint32_t ebp = cc.dw(0);

	cc.org_text(0x100);
	cc.ldr(rSP, rPC, sp_dw);
	cc.mov(r2, 0U);
	cc.mov(r3, 0U);

	const uint32_t sample_loop0 = cc.ldr(r1, rPC, cinstr);
	const uint32_t sample_loop1 = cc.ldr(r0, rPC, ebp);
	const uint32_t sample_loop2 = cc.add(r0, r0, 1);
	cc.str(r0, rPC, ebp);
	cc.cmp(r0, r1);
	cc.bne(sample_loop1);
	cc.add(r2, r2, 1);
	cc.bne(sample_loop2);
	cc.add(r3, r3, 1);
	cc.b(sample_loop0);
}

void test_armvm_perf(armvm_ref armvm, unsigned seconds)
{
	const uint64_t icount_start = armvm_spr64(armvm, ARMVM_SPR64(ICOUNT));

	const uint64_t dtime_second = dtime_calibrate();
	const double kips_ratio = 1.0 / KHz(1.0);
	const double mips_ratio = 1.0 / MHz(1.0);

	uint64_t t0 = get_dtime(), t1 = 0;
	do {
		t1 = get_dtime();
	}while(t0 == t1);

	const uint64_t dtime_start = t1;
	const uint64_t dtime_stop = dtime_start + (dtime_second * seconds);

	do {
		armvm_core_step(armvm->core);
	}while(dtime_stop > get_dtime());

	const uint64_t dtime_end = get_dtime();
	const uint64_t delta_dtime_run = dtime_end - dtime_start;

	const uint64_t icount = armvm_spr64(armvm, ARMVM_SPR64(ICOUNT)) - icount_start;

	LOG_START("dtime_start: 0x%016" PRIx64, dtime_start);
	LOG_END(", dtime_stop: 0x%016" PRIx64, dtime_stop);
	LOG_START("dtime_end: 0x%016" PRIx64, dtime_end);
	LOG_END(", delta_dtime_run: 0x%016" PRIx64, delta_dtime_run);

	const uint64_t icount_second = icount / seconds;

	LOG("ICOUNT: 0x%016" PRIx64 ", seconds: %u, per second: %" PRIu64,
		icount, seconds, icount_second);

	if(icount_second < MHz(1)) {
		LOG("KIPS: %0.16f", icount_second * kips_ratio);
	} else {
		LOG("MIPS: %0.16f", icount_second * mips_ratio);
	}
}

int main(void)
{
	armvm_ptr armvm = 0;

	uint8_t mem[kTEST_MEM_ALLOC];

	armvm_alloc_init(armvm_alloc(&armvm));

//	armvm->core->config.trace = 1;

	armvm_mem_mmap_rw(armvm->mem, kTEST_MEM_START, kTEST_MEM_END, mem);

	test_armvm_perf__setup(mem);
	test_armvm_perf(armvm, 1);
	test_armvm_perf(armvm, 10);
	test_armvm_perf(armvm, 60);

	return(0);
}
