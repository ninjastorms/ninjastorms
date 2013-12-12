
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
      if (*c == '%')
        {
          ++c;
          switch (*c)
            {
            case '%':
              if (__builtin_expect(putchar('%') == EOF, 0))
                return EOF;
              break;
            case 'c':
              {
                int x = va_arg(ap, int);
                if (__builtin_expect(putchar(x) == EOF, 0))
                  return EOF;
              }
              break;
            default:
              printf("\nprintf: unimplemented conversion character '%c'\n", *c);
              return -1;
              break;
            }
        }
      if (__builtin_expect(putchar(*c) == EOF, 0))
        return EOF;
      ++chars_written;
      ++c;
    }
  
  return chars_written;
}
