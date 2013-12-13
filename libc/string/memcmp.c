
#include <stddef.h>

int 
memcmp (const void *s1, const void *s2, size_t n)
{
  const char *c1 = s1;
  const char *c2 = s2;

  size_t i;
  for (i = 0; i < n; ++i)
    {
      int res = ((int)c1[i]) - c2[i];
      if (res)
        return res;
    }

  return 0;
}
