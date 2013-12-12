
#pragma once

/*
 * Our subset of stdio.h
 */

#include <stdarg.h>

#ifndef EOF
# define EOF (-1)
#endif


int putchar(int c);

int puts(const char * __restrict s);

int printf(const char * __restrict format, ...);

int vprintf(const char * __restrict format, va_list ap);
