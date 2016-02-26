
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#include <errno.h>

int
vprintf (const char *format, __attribute__((unused)) va_list ap)
{
  const char *c = format;

  unsigned int chars_written = 0;

  while (*c)
    {
      if (*c == '%')
        {
          ++c;
          switch (*c)
            {
            case '%':
              if (__builtin_expect(putchar('%') == EOF, 0))
                return chars_written;
              ++chars_written;
              break;
            case 'c':
              {
                int x = va_arg(ap, int);
                if (__builtin_expect(putchar(x) == EOF, 0))
                  return chars_written;
                ++chars_written;
              }
              break;
            case 's':
              {
                char *x = va_arg(ap, char*);
                if (__builtin_expect(x == NULL, 0))
                  {
                    int res = printf("(null)");
                    if (__builtin_expect(res == EOF, 0))
                      return chars_written;
                    chars_written += res;
                  }
                else
                  {
                    while (*x)
                      {
                        if (__builtin_expect(putchar(*x) == EOF, 0))
                          return chars_written;
                        ++x;
                        ++chars_written;
                      }
                  }
              }
              break;
            case 'i':
              {
                int x = va_arg(ap, int);
                char tmp[10] = { 0 };
                int i = 0;
                int sign = x < 0;
                chars_written += sign;
                if (sign)
                  x *= -1;
                while (x)
                  {
                    tmp[i] = x % 10;
                    x /= 10;
                    ++i;
                  }
                if (i != 0)
                  --i;
                if (sign)
                  if (__builtin_expect(putchar('-') == EOF, 0))
                    return chars_written;
                for ( ; i >= 0; --i)
                  {
                      if (__builtin_expect(putchar(tmp[i] + '0') == EOF, 0))
                        return chars_written;
                    ++chars_written;
                  }
              }
              break;
            case 'x':
              {
                unsigned int x = va_arg(ap, int);
                char tmp[10] = { 0 };
                int i = 0;
                while (x)
                  {
                    tmp[i] = x % 0x10;
                    x /= 0x10;
                    ++i;
                  }
                if (i != 0)
                  --i;
                for ( ; i >= 0; --i)
                  {
                        if (tmp[i] > 9)
                          {
                            if (__builtin_expect(putchar(tmp[i] + 'a' - 10) == EOF, 0))
                              return chars_written;
                          }
                        else
                          {
                            if (__builtin_expect(putchar(tmp[i] + '0') == EOF, 0))
                              return chars_written;
                          }
                    ++chars_written;
                  }
              }
              break;
            case 'X':
              {
                int x = va_arg(ap, int);
                char tmp[10] = { 0 };
                int i = 0;
                while (x)
                  {
                    tmp[i] = x % 0x10;
                    x /= 0x10;
                    ++i;
                  }
                if (i != 0)
                  --i;
                for ( ; i >= 0; --i)
                  {
                        if (tmp[i] > 9)
                          {
                            if (__builtin_expect(putchar(tmp[i] + 'A' - 10) == EOF, 0))
                              return chars_written;
                          }
                        else
                          {
                            if (__builtin_expect(putchar(tmp[i] + '0') == EOF, 0))
                              return chars_written;
                          }
                    ++chars_written;
                  }
              }
              break;

            default:
              printf("\nprintf: unimplemented conversion character '%c'\n", *c);
              return chars_written;
              break;
            }
          ++c;
        }
      else
        {
          if (__builtin_expect(putchar(*c) == EOF, 0))
            return chars_written;
          ++chars_written;
          ++c;
        }
    }

  return chars_written;
}
