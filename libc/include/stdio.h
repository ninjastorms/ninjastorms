
#pragma once

/*
 * Our subset of stdio.h
 */

#include <stdarg.h>

#ifndef EOF
# define EOF (-1)
#endif

#ifndef __may_fail
# define __may_fail __attribute__((warn_unused_result))
#endif

#ifndef __check_format
# define __check_format __attribute__((format (printf, 1, 2)))
#endif

int putchar(int c);

int puts(const char * __restrict s);

int printf(const char * __restrict format, ...) __check_format;

int vprintf(const char * __restrict format, va_list ap);
