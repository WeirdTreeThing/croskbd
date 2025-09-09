#ifndef UTILS_H
#define UTILS_H

#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

typedef enum {
  LOG_DEBUG,
  LOG_WARN,
  LOG_ERROR,
  LOG_LEN,
} LOG_LEVEL;

void log_msg(LOG_LEVEL level, const char * fmt, ...);

#define dbg(...) log_msg(LOG_DEBUG, __VA_ARGS__)
#define warn(...) log_msg(LOG_WARN, __VA_ARGS__)
#define err(...) log_msg(LOG_ERROR, __VA_ARGS__)

#endif
