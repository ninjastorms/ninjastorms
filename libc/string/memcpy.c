
#include <stddef.h>

void *
memcpy (void *dest, const void *src, size_t n)
{
  size_t i;
  for (i = 0; i < n; ++i)
    ((unsigned char*)dest)[i] = ((const unsigned char*)src)[i];

  return dest;
}
