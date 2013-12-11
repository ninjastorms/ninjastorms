
#pragma once

/*
 * Our subset of stddef.h
 */

#include <sys/types.h>

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
