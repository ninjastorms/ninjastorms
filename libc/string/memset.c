
#include <string.h>

void*
memset (void *s, int c, size_t n)
{
  void *i;
  for (i = s; i < s + n; ++i)
    *((unsigned char*)i) = c;
 
  return s;
}
