#include <croskbd.h>
#include <evdev.h>
#include <linux/input.h>
#include <poll.h>
#include <stdio.h>
#include <uinput.h>
#include <unistd.h>

Settings settings = {
    .invert_top_row = 0,
    .handle_tablet_switch = 1,
    .del_key = 1,
};
KeyboardDevice kdev = {
    .fd = -1,
};
TabletSwitchDevice tdev = {
    .fd = -1,
    .tablet_mode = 0,
};
UInputDevice udev = {.fd = -1};

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

  while (1) {
    if (poll(pfds, nfds, -1) < 0) {
      perror("poll");
      return;
    }
    if (pfds[1].revents) {
      read(tdev.fd, &ts_ev, sizeof(ts_ev));
      printf("Event --------------\nev.code=%d ev.value=%d\n", ts_ev.code,
             ts_ev.value);
    }
    if (pfds[0].revents) {
      read(kdev.fd, &kb_ev, sizeof(kb_ev));
      printf("Event --------------\nev.code=%d ev.value=%d\n", kb_ev.code,
             kb_ev.value);
    }
  }
}

int main(int argc, char **argv) {
  scan_input_devices(&kdev, &tdev);

  if (kdev.fd < 0) {
    printf("Failed to find keyboard device\n");
    return (1);
  }

  uinput_init(&udev);

  if (udev.fd < 0) {
    printf("Failed to create virtual keyboard\n");
    return 1;
  }

  main_loop();

  return 0;
}
