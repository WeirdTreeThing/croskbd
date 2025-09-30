#include <config.h>
#include <croskbd.h>
#include <errno.h>
#include <evdev.h>
#include <linux/input.h>
#include <poll.h>
#include <remap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uinput.h>
#include <unistd.h>
#include <utils.h>

Settings settings = {
    .invert_top_row = 0,
    .handle_tablet_switch = 1,
    .delete_key = 1,
    .debug = 0,
    .override_key_codes = 1,
};
KeyboardDevice kdev = {
    .fd = -1,
    .has_vivaldi = 0,
    .remaps = {0},
    .num_remaps = 0,
};
TabletSwitchDevice tdev = {
    .fd = -1,
    .tablet_mode = 0,
};
UInputDevice udev = {.fd = -1};

void cleanup() {
  dbg("Exiting...");
  close_dev_fds(&kdev, &tdev);
  uinput_teardown(&udev);
}

void input_loop(void) {
  int nfds = 1;
  struct pollfd pfds[2];
  struct input_event kb_ev;
  struct input_event ts_ev;

  pfds[0].fd = kdev.fd;
  pfds[0].events = POLLIN;

  if (tdev.fd > 1) {
    pfds[1].fd = tdev.fd;
    pfds[1].events = POLLIN;
    nfds++;
  } else {
    warn("Tablet switch device not found, disabling tablet mode switch handling.");
  }

  // block all keyboard events to other processes
  ioctl(kdev.fd, EVIOCGRAB);

  while (1) {
    if (poll(pfds, nfds, -1) < 0) {
      err("poll failed: %s", strerror(errno));
      return;
    }
    if (pfds[1].revents) {
      read(tdev.fd, &ts_ev, sizeof(ts_ev));
    }
    if (pfds[0].revents) {
      int ret = read(kdev.fd, &kb_ev, sizeof(kb_ev));
      if (ret < 0) {
        err("read() returned %d bytes", ret);
        return;
      }
      process_key(&kdev, &udev, &kb_ev);
    }
  }
}

int main(int argc, char **argv) {
  atexit(cleanup);
  signal(SIGTERM, exit);
  signal(SIGINT, exit);

  parse_config();

  input_device dev = {};

  uinput_init(&udev);

  if (udev.fd < 0) {
    err("Failed to create virtual keyboard");
    return 1;
  }

  while (1) {
    int ret = get_keyboards(&dev);

    if (ret) {
      kdev.fd = dev.fd;
      snprintf(kdev.ev_name, sizeof(kdev.ev_name), "%s", dev.event_name);
      load_kb_layout_data(&kdev);
      add_remaps(&kdev);
      input_loop();
    } else {
      err("Failed to find keyboard device");
    }
  }

  return 0;
}
