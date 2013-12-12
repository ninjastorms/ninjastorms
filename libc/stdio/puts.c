
#include <stdio.h>

int
puts (const char *s)
{
  const char *c = s;
  while (*c)
    {
      int res = putchar(*c);
      if (res == EOF)
        return EOF;
      ++c;
    }
  
  int res = putchar('\n');
  if (res == EOF)
    return EOF;

  return 0;
}
