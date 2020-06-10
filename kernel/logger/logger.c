#include <stdio.h>
#include "logger.h"

void general_log(uint32_t severity, char log_line[], const char* file)
  {
    printf("%s[%s]\x1b[0m [%s]: %s\n", log_colors[severity], log_severities[severity], file, log_line);
  }

void general_log_full_colorized(uint32_t severity, char log_line[], const char* file)
  {
    printf("%s[%s] [%s] %s \x1b[0m\n", log_colors[severity], log_severities[severity], file, log_line);
  }