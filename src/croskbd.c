#include <croskbd.h>
#include <evdev.h>
#include <linux/input.h>
#include <poll.h>
#include <remap.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <uinput.h>
#include <unistd.h>

Settings settings = {
    .invert_top_row = 0,
    .handle_tablet_switch = 1,
    .del_key = 1,
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
  printf("\nExiting...\n");
  close_dev_fds(&kdev, &tdev);
  uinput_teardown(&udev);
}

void main_loop(void) {
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
    printf("Tablet switch device not found, disabling tablet mode switch "
           "handling.\n");
  }

  // block all keyboard events to other processes
  ioctl(kdev.fd, EVIOCGRAB);

  while (1) {
    if (poll(pfds, nfds, -1) < 0) {
      perror("poll");
      return;
    }
    if (pfds[1].revents) {
      read(tdev.fd, &ts_ev, sizeof(ts_ev));
    }
    if (pfds[0].revents) {
      read(kdev.fd, &kb_ev, sizeof(kb_ev));
      int keycode = remap_key(&kdev, kb_ev.code);
      uinput_send_event(&udev, kb_ev.type, keycode, kb_ev.value);
    }
  }
}

int main(int argc, char **argv) {
  atexit(cleanup);
  signal(SIGTERM, exit);
  signal(SIGINT, exit);
  scan_input_devices(&kdev, &tdev);

  if (kdev.fd < 0) {
    printf("Failed to find keyboard device\n");
    return (1);
  }

  load_kb_layout_data(&kdev);
  for (int i = 0; i < kdev.num_top_row_keys; i++) {
    printf("Top row keycode: %d\n", kdev.top_row_keys[i]);
  }
  generate_remaps(&kdev);
  for (int i = 0; i < kdev.num_remaps; i++) {
    printf("remap: original=%d remap=%d\n", kdev.remaps[i].original_key,
           kdev.remaps[i].remap_key);
  }

  uinput_init(&udev);

  if (udev.fd < 0) {
    printf("Failed to create virtual keyboard\n");
    return 1;
  }

  main_loop();

  return 0;
}
