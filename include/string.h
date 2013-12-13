
#pragma once

/*
 * Our subset of string.h
 */

#include <stddef.h>

void *memset(void * __restrict s, int c, size_t n);

void *memcpy(void * __restrict dest, const void * __restrict src, size_t n); 

int memcmp(const void *s1, const void *s2, size_t n);
