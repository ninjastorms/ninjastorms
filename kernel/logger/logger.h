#pragma once

#include <sys/types.h>

#define log_debug(log_line) general_log(0, log_line, __FILE__);
#define log_info(log_line) general_log(1, log_line, __FILE__);
#define log_warn(log_line) general_log(2, log_line, __FILE__);
#define log_error(log_line) general_log(3, log_line, __FILE__);
#define log_fatal(log_line) general_log_full_colorized(4, log_line, __FILE__);

void general_log(uint32_t severity, char log_line[], const char* file);
void general_log_alt(uint32_t severity, char log_line[], const char* file);

static const char* log_severities[] =
  {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
  };

static const char* log_colors[] =
  {
    "\x1b[37m", // white
    "\x1b[34m", // blue
    "\x1b[33m", // yellow
    "\x1b[91m", // light red
    "\x1b[31m", // red
  };
