#include <croskbd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>

void uinput_send_event(UInputDevice *udev, int type, int code, int value) {
  struct input_event ev;
  ev.type = type;
  ev.code = code;
  ev.value = value;
  ev.time.tv_sec = 0;
  ev.time.tv_usec = 0;
  write(udev->fd, &ev, sizeof(ev));
}

void uinput_teardown(UInputDevice *udev) {
  if (udev->fd) {
    ioctl(udev->fd, UI_DEV_DESTROY);
    close(udev->fd);
  }
}

void uinput_init(UInputDevice *udev) {
  struct uinput_setup usetup;

  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if (fd < 0) {
    err("Failed to open uinput: %s", strerror(errno));
    exit(1);
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

  udev->fd = fd;
}
