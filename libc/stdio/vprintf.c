
#include <stdio.h>
#include <stdarg.h>

#include <errno.h>

int
vprintf (const char *format, __attribute__((unused)) va_list ap)
{
  const char *c = format;
 
  unsigned int chars_written = 0;

  while (*c)
    {
      int res = putchar(*c);
      if (__builtin_expect(res == EOF, 0))
        return EOF;
      ++chars_written;
      ++c;
    }
  
  return chars_written;
}
