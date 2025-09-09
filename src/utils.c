#include <stdarg.h>
#include <stdio.h>
#include <utils.h>

char * levels[LOG_LEN] = {
  "DEBUG",
  "WARN",
  "ERROR"
};

char * colors[LOG_LEN] = {
  COLOR_BLUE,
  COLOR_YELLOW,
  COLOR_RED,
};

void log_msg(LOG_LEVEL level, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("%s[%s] ", colors[level], levels[level]);
  vfprintf(stdout, fmt, args);
  printf("%s\n", COLOR_RESET);
}
