#include <croskbd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utils.h>

int convert_scancode_to_keycode(int fd, int scancode) {
	struct input_keymap_entry entry = {
		.flags = 0,
		.len = sizeof(scancode),
		.keycode = 0,
	};

	memcpy(entry.scancode, &scancode, sizeof(scancode));
	int ret = ioctl(fd, EVIOCGKEYCODE_V2, &entry);
	if (ret < 0) {
		err("EVIOCGKEYCODE_V2: %s", strerror(errno));
		return 0;
	}

	return entry.keycode;
}

int load_kb_vivaldi_data(KeyboardDevice *kdev) {
	char data_path[100];
	char function_row_physmap[50] = {0};
	snprintf(data_path, sizeof(data_path),
			 "/sys/class/input/%s/device/device/function_row_physmap",
			 kdev->ev_name);

	int fd = open(data_path, O_RDONLY);
	if (fd < 0) {
		err("open vivaldi data failed: %s", strerror(errno));
		return 0;
	}
	kdev->has_vivaldi = 1;
	read(fd, function_row_physmap, sizeof(function_row_physmap));

	char *tok = strtok(function_row_physmap, " ");
	int count = 0;
	while (tok != NULL) {
		int scancode = strtol(tok, NULL, 16);
		int keycode = convert_scancode_to_keycode(kdev->fd, scancode);
		kdev->top_row_keys[count] = keycode;
		tok = strtok(NULL, " ");
		count++;
	}
	kdev->num_top_row_keys = count;

	return 1;
}
