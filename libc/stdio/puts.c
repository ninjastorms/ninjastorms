
#include <stdio.h>

int
puts (const char *s)
{
  const char *c = s;
  while (*c)
    {
      int res = putchar(*c);
      if (__builtin_expect(res == EOF, 0))
        return EOF;
      ++c;
    }
  
  int res = putchar('\n');
  if (__builtin_expect(res == EOF, 0))
    return EOF;

  return 0;
}
