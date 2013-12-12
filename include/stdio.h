
#pragma once

/*
 * Our subset of stdio.h
 */

#include <stdarg.h>

#ifndef EOF
# define EOF (-1)
#endif


int putchar(int c);

int puts(const char *s);

int printf(const char *format, ...);

int vprintf(const char *format, va_list ap);
