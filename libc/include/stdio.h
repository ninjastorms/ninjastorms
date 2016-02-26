
/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#pragma once

/*
 * Our subset of stdio.h
 */

#include <stdarg.h>

#ifndef EOF
#  define EOF (-1)
#endif

#ifndef __may_fail
#  define __may_fail __attribute__((warn_unused_result))
#endif

#ifndef __check_format
#  define __check_format __attribute__((format (printf, 1, 2)))
#endif

int putchar(int c);

int puts (const char * __restrict s);

int printf (const char * __restrict format, ...) __check_format;

int vprintf (const char * __restrict format, va_list ap);
