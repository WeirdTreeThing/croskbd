#include <croskbd.h>
#include <evdev.h>
#include <stdio.h>
#include <uinput.h>

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

  evloop(&kdev);

  return 0;
}
