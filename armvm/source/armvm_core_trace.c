#include "armvm_core_trace.h"

/* **** */

#include "armvm_trace_glue.h"
#include "armvm_trace.h"

/* **** */

#include "git/libarm/include/arm_ir.h"
#include "git/libarm/include/arm_strings.h"

/* **** */

#include <stdarg.h>
#include <stdio.h>

/* **** */

void __trace_end(armvm_trace_p const atp)
{ if(atp) printf(")\n"); }

int __trace_start(armvm_trace_p const atp)
{
	if(!atp) return(0);

	int thumb = IP & 1;

//	printf("%c", thumb ? 'T' : 'A');
//	printf("(0x%08x(0x%08x):%s: ", IP & (~1 << (1 >> thumb)), IR, arm_cc_ucase_string[1][rSPR32(CC)]);
	printf("%c(0x%08x(0x%08x):%s(%c): ",
		thumb ? 'T' : 'A',
		IP & (~1U << (1 >> thumb)), IR,
		arm_cc_ucase_string[1][rSPR32(CC)],
		CCX ? '>' : 'X');

	return(1);
}

void _armvm_trace_(armvm_trace_p const atp, const char* format, ...)
{
	if(!atp) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

/* **** */

void _armvm_trace_comment(armvm_trace_p const atp, const char* format, ...)
{
	if(!atp) return;

	printf("; /* ");

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	printf(" */");
}

void _armvm_trace_end(armvm_trace_p const atp, const char* format, ...)
{
	if(!atp) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(atp);
}

void _armvm_trace_start(armvm_trace_p const atp, const char* format, ...)
{
	if(!atp) return;

	__trace_start(atp);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void _armvm_trace(armvm_trace_p const atp, const char* format, ...)
{
	if(!atp) return;

	__trace_start(atp);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(atp);
}
