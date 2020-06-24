#pragma once

#include <sys/types.h>

#define log_debug(...) general_log(0, __FILE__, __VA_ARGS__);
#define log_info(...) general_log(1, __FILE__, __VA_ARGS__);
#define log_warn(...) general_log(2, __FILE__, __VA_ARGS__);
#define log_error(...) general_log(3, __FILE__, __VA_ARGS__);
#define log_fatal(...) general_log(4, __FILE__, __VA_ARGS__);

void general_log(uint32_t severity, const char* file, const char* format, ...);

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
