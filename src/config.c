#include <croskbd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>

enum states {
	STATE_START_NAME,
	STATE_READ_NAME,
	STATE_AFTER_NAME,
	STATE_START_VAL,
	STATE_READ_VAL,
	STATE_AFTER_VAL
};

#define is_whitespace(c) (c == ' ' || c == '\t' || c == '\n')
#define is_whitespace_excl_nl(c) (c == ' ' || c == '\t')

char buffer[100] = { 0 };
char config_name[100] = { 0 };
int buffer_pos = 0;

extern Settings settings;

static void push_char(char c) {
	if (buffer_pos >= sizeof(buffer)-1) {
		warn("config buffer too full");
		return;
	}
	buffer[buffer_pos] = c;
	buffer_pos++;
}

static void clear_buffer() {
	memset(buffer, 0, sizeof(buffer));
	buffer_pos = 0;
}

static void process_config() {
	int value;
	if (!strncmp(buffer, "true", sizeof(buffer))) {
		value = 1;
	} else if (!strncmp(buffer, "false", sizeof(buffer))) {
		value = 0;
	} else {
		err("Invalid value %s for config %s", buffer, config_name);
		return;
	}
	if (!strncmp(config_name, "invert_top_row", sizeof(config_name)))
		settings.invert_top_row = value;
	else if (!strncmp(config_name, "handle_tablet_switch", sizeof(config_name)))
		settings.handle_tablet_switch = value;
	else if (!strncmp(config_name, "delete_key", sizeof(config_name)))
		settings.delete_key = value;
	else if (!strncmp(config_name, "debug", sizeof(config_name)))
		settings.debug = value;
	else if (!strncmp(config_name, "override_key_codes", sizeof(config_name)))
		settings.override_key_codes = value;
	else
		err("Invalid config %s", config_name);
}

void parse_config() {
	char config[] = "./example.conf";
	dbg("Reading config");
	int fd = open(config, O_RDONLY);
	if (fd < 0) {
		err("Failed to open config %s: %s", config, strerror(errno));
		return;
	}

	int size = lseek(fd, 0, SEEK_END) + 1; // Include extra null byte
	lseek(fd, 0, 0);
	char *data = malloc(size);
	int ret = read(fd, data, size - 1);
	if (ret < 0) {
		err("Failed to read config: %s", strerror(errno));
		return;
	}

	enum states state = STATE_START_NAME;

	for (int i = 0; i < size; i++) {
		switch (state) {
			case STATE_START_NAME:
				if (!is_whitespace(data[i])) {
					state = STATE_READ_NAME;
					push_char(data[i]);
				}
				break;
			case STATE_READ_NAME:
				if (data[i] == '=') {
					state = STATE_START_VAL;
					memcpy(config_name, buffer, sizeof(config_name));
					clear_buffer();
				} else if (is_whitespace(data[i])) {
					state = STATE_AFTER_NAME;
					memcpy(config_name, buffer, sizeof(config_name));
					clear_buffer();
				} else {
					push_char(data[i]);
				}
				break;
			case STATE_AFTER_NAME:
				if (data[i] == '=')
					state = STATE_START_VAL;
				break;
			case STATE_START_VAL:
				if (!is_whitespace(data[i])) {
					state = STATE_READ_VAL;
					push_char(data[i]);
				}
				break;
			case STATE_READ_VAL:
				if (data[i] == '\n') {
					state = STATE_START_NAME;
					dbg("config_name=%s value=%s", config_name, buffer);
					process_config();
					memset(config_name, 0 , sizeof(config_name));
					clear_buffer();
				} else if (is_whitespace_excl_nl(data[i])) {
					state = STATE_AFTER_VAL;
					dbg("config_name=%s value=%s", config_name, buffer);
					process_config();
					memset(config_name, 0, sizeof(config_name));
					clear_buffer();
				} else {
					push_char(data[i]);
				}				
				break;
			case STATE_AFTER_VAL:
				if (data[i] == '=')
					state = STATE_START_NAME;
				break;
		}
	}
	free(data);
}
