
#include <stdio.h>
#include <stdarg.h>

int
printf (const char* format, ...)
{
  va_list arg;
  int res;

  va_start (arg, format);
  res = vprintf(format, arg);
  va_end (arg);

  return res;
}  
