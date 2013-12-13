
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
                while (x)
                  {
                    tmp[i] = x % 10;
                    x /= 10;
                    ++i;
                  }
                for ( ; i >= 0; --i)
                  {
                    if (tmp[i] != 0 || i == 0)
                      if (__builtin_expect(putchar(tmp[i] + '0') == EOF, 0))
                        return chars_written;
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
