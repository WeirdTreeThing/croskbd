#include <croskbd.h>
#include <stdarg.h>
#include <stdio.h>
#include <utils.h>

extern Settings settings;

char *levels[LOG_LEN] = {"DEBUG", "WARN", "ERROR"};

char *colors[LOG_LEN] = {
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_RED,
};

void log_msg(LOG_LEVEL level, const char *fmt, ...) {
	if (!settings.debug && level == LOG_DEBUG)
		return;
	va_list args;
	va_start(args, fmt);
	printf("%s[%s] ", colors[level], levels[level]);
	vfprintf(stdout, fmt, args);
	printf("%s\n", COLOR_RESET);
}
