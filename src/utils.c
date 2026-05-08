#include <croskbd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
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

// Returns NULL if some error occured
// Returns a string if some error didn't occur
// The caller of this function must free the memory returned
char *read_to_string(const char *path) {
	int ret, fd, size;
	char *buf = NULL;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		err("Failed to open %s: %s", path, strerror(errno));
		return NULL;
	}

	size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, 0);

	buf = malloc(size);
	if (buf == NULL) {
		err("Failed to allocate buffer for %s: %s", path, strerror(errno));
		goto fail;
	}

	ret = read(fd, buf, size);
	if (ret < 0) {
		err("Failed to read %s: %s", path, strerror(errno));
		goto fail_buf;
	}

	close(fd);
	return buf;

fail_buf:
	free(buf);

fail:
	close(fd);
	return NULL;
}
