#include <linux/uinput.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int uinput_init(void) {
	struct uinput_setup usetup;

	int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("Failed to open uinput");
		return -1;
	}

	ioctl(fd, UI_SET_EVBIT, EV_REP);
	ioctl(fd, UI_SET_EVBIT, EV_KEY);
	for (int code = 0; code < 255; code++) {
		ioctl(fd, UI_SET_KEYBIT, code);
	}

	memset(&usetup, 0, sizeof(usetup));
	usetup.id.bustype = BUS_USB;
	usetup.id.vendor = 0xABCD;
	usetup.id.product = 0x1234;
	strcpy(usetup.name, "Chromebook Keyboard");

	ioctl(fd, UI_DEV_SETUP, &usetup);
	ioctl(fd, UI_DEV_CREATE);

	return fd;
}
