
#pragma once

/*
 * Our subset of stdio.h
 */

#ifndef EOF
# define EOF (-1)
#endif


int putchar(int c);

int puts(const char *s);


// the following are still provided by u-boot
void printf(const char*, ...);

