#include "armvm_core_trace.h"

/* **** */

//#include "armvm_trace_glue.h"
#include "armvm_trace.h"

#include "armvm_core_glue.h"

/* **** */

#include "libarm/include/arm_cpsr.h"
#include "libarm/include/arm_ir.h"
#include "libarm/include/arm_strings.h"

/* **** */

#include <stdarg.h>
#include <stdio.h>

/* **** */

void __trace_end(armvm_core_ref core)
{ if(core) printf(")\n"); }

int __trace_start(armvm_core_ref core)
{
	if(!core->config.trace) return(0);

	char flags[5], *dst = flags;

	*dst++ = IF_CPSR(C) ? 'C' : 'c';
	*dst++ = IF_CPSR(N) ? 'N' : 'n';
	*dst++ = IF_CPSR(V) ? 'V' : 'v';
	*dst++ = IF_CPSR(Z) ? 'Z' : 'z';
	*dst = 0;

//	const unsigned thumb = ARM_CPSR_BEXT(Thumb); //  should be the CORRECT way...
	const unsigned thumb = IP & 1; // the STUPID KLUDGE way...  UGH!!!

	printf("%c(0x%08x(0x%08x):%s:%s(%c): ",
		thumb ? 'T' : 'A',
		IP & ~(3U >> thumb), IR,
		flags,
		arm_cc_ucase_string[1][rSPR32(CC)],
		CCX ? '>' : 'X');

	return(1);
}

static
void __trace_comment_end(armvm_core_ref core)
{
	if(!core) return;
	if(!core->config.trace) return;

	printf(" */");
}

static
int __trace_comment_start(armvm_core_ref core)
{
	if(!core) return(0);
	if(!core->config.trace) return(0);

	printf(" /* ");

	return(1);
}


/* **** */

void _armvm_trace_(armvm_core_ref core, const char* format, ...)
{
	if(!core->config.trace) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

/* **** */

void _armvm_trace_comment(armvm_core_ref core, const char* format, ...)
{
	if(!__trace_comment_start(core)) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_comment_end(core);
}

void _armvm_trace_end(armvm_core_ref core, const char* format, ...)
{
	if(!core->config.trace) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(core);
}

void _armvm_trace_end_with_comment(armvm_core_ref core, const char* format, ...)
{
	if(!__trace_comment_start(core)) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_comment_end(core);
	__trace_end(core);
}

int _armvm_trace_start(armvm_core_ref core, const char* format, ...)
{
	if(!core->config.trace) return(0);

	__trace_start(core);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	return(1);
}

void _armvm_trace(armvm_core_ref core, const char* format, ...)
{
	if(!core->config.trace) return;

	__trace_start(core);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(core);
}
