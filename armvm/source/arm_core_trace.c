#include "armvm_core_trace.h"

/* **** */

#include "arm_trace_glue.h"
#include "arm_trace.h"

/* **** */

#include "arm_ir.h"
#include "arm_strings.h"

/* **** */

#include <stdarg.h>
#include <stdio.h>

/* **** */

void __trace_end(arm_trace_p atp)
{ if(atp) printf(")\n"); }

int __trace_start(arm_trace_p atp)
{
	if(!atp) return(0);

	int thumb = IP & 1;

//	printf("%c", thumb ? 'T' : 'A');
//	printf("(0x%08x(0x%08x):%s: ", IP & (~1 << (1 >> thumb)), IR, arm_cc_ucase_string[1][rSPR32(CC)]);
	printf("%c(0x%08x(0x%08x):%s(%c): ",
		thumb ? 'T' : 'A',
		IP & (~1 << (1 >> thumb)), IR,
		arm_cc_ucase_string[1][rSPR32(CC)],
		CCX ? '>' : 'X');

	return(1);
}

void _arm_trace_(arm_trace_p atp, const char* format, ...)
{
	if(!atp) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

/* **** */

void _arm_trace_comment(arm_trace_p atp, const char* format, ...)
{
	if(!atp) return;

	printf("; /* ");

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	printf(" */");
}

void _arm_trace_end(arm_trace_p atp, const char* format, ...)
{
	if(!atp) return;

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(atp);
}

void _arm_trace_start(arm_trace_p atp, const char* format, ...)
{
	if(!atp) return;

	__trace_start(atp);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void _arm_trace(arm_trace_p atp, const char* format, ...)
{
	if(!atp) return;

	__trace_start(atp);

	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);

	__trace_end(atp);
}
