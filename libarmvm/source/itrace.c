#define pCORE core

#include "armvm_core.h"

/* **** */

#include "libarm/include/apsr/apsr.h"
#include "libarm/include/cpsr/cpsr.h"
#include "libarm/include/ir.h"
#include "libarm/include/strings.h"

/* **** */

#include <stdarg.h>
#include <stdio.h>

/* **** */

static __attribute__((warn_unused_result))
int ___itrace_check(armvm_core_ref core)
{ return(core && core->config.trace); }

/* **** */

static
int __itrace_comment_end(armvm_core_ref core, const char *const format, va_list ap)
{
	if(!___itrace_check(core)) return(0);

	if(format)
		vprintf(format, ap);

	printf(" */ ");

	return(1);
}

static __attribute__((warn_unused_result))
int __itrace_comment_start(armvm_core_ref core, const char *const format, va_list ap)
{
	if(!___itrace_check(core)) return(0);

	printf(" /* ");

	if(format)
		vprintf(format, ap);

	return(1);
}

static
int __itrace_comment_start_end(armvm_core_ref core, const char *const format, va_list ap)
{
	const int rval = __itrace_comment_start(core, format, ap);
	if(rval)
		__itrace_comment_end(core, 0, ap);

	return(rval);
}

static
void __itrace_end(armvm_core_ref core, const char *const format, va_list ap)
{
	if(!___itrace_check(core)) return;

	if(format)
		vprintf(format, ap);

	printf(")\n");
}

static __attribute__((warn_unused_result))
int __itrace_start(armvm_core_ref core, const char *const format, va_list ap)
{
	if(!___itrace_check(core)) return(0);

	char flags[5], *dst = flags;

	*dst++ = APSR(c) ? 'C' : 'c';
	*dst++ = APSR(n) ? 'N' : 'n';
	*dst++ = APSR(v) ? 'V' : 'v';
	*dst++ = APSR(z) ? 'Z' : 'z';
	*dst = 0;

//	const unsigned thumb = ARM_CPSR_BEXT(Thumb); //  should be the CORRECT way...
	const unsigned thumb = IP & 1; // the STUPID KLUDGE way...  UGH!!!

	printf("%c(0x%08x(0x%08x):%s:%s(%c): ",
		thumb ? 'T' : 'A',
		IP & ~(3U >> thumb), IR,
		flags,
		arm_cc_ucase_string[1][CCv],
		CCX ? '>' : 'X');

	if(format)
		vprintf(format, ap);

	return(1);
}

static
void __itrace_start_end(armvm_core_ref core, const char *const format, va_list ap)
{
	if(__itrace_start(core, format, ap))
		__itrace_end(core, 0, ap);
}

/* **** */

void _itrace_(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	if(___itrace_check(core))
		vprintf(format, ap);

	va_end(ap);
}

/* **** */

void _itrace_comment(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	__itrace_comment_start_end(core, format, ap);

	va_end(ap);
}

void _itrace_end(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	__itrace_end(core, format, ap);

	va_end(ap);
}

void _itrace_end_with_comment(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	if(__itrace_comment_start_end(core, format, ap))
		__itrace_end(core, 0, ap);

	va_end(ap);
}

/* **** */

void itrace(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	__itrace_start_end(core, format, ap);

	va_end(ap);
}

int itrace_start(armvm_core_ref core, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	const int rval = __itrace_start(core, format, ap);

	va_end(ap);

	return(rval);
}

