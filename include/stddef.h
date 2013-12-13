
#pragma once

/*
 * Our subset of stddef.h
 */

#include <sys/types.h>

#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

#define NULL ((void*)0)
