#include <stdio.h>
#include <stdarg.h>
#include "logger.h"

void 
general_log(uint32_t severity, const char* file, const char* format, ...)
{
  va_list args;

  // Align shorter severities on the same level
  if(severity == 1 || severity == 2) 
    printf("%s[%s] \x1b[0m [%s]: ", log_colors[severity], log_severities[severity], file);
  else 
    printf("%s[%s]\x1b[0m [%s]: ", log_colors[severity], log_severities[severity], file);
    
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  putchar('\n');
}
