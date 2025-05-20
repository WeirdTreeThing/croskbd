#include <croskbd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int convert_scancode_to_keycode(int fd, int scancode) {
  struct input_keymap_entry entry = {
      .flags = 0,
      .len = sizeof(scancode),
      .keycode = 0,
  };

  int ret = ioctl(fd, EVIOCGKEYCODE_V2, &entry);
  if (ret < 0) {
    perror("EVIOCGKEYCODE_V2");
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
    perror("open vivaldi data");
    return 0;
  }
  read(fd, function_row_physmap, sizeof(function_row_physmap));

  char *tok = strtok(function_row_physmap, " ");
  while (tok != NULL) {
    int scancode = strtol(tok, NULL, 16);
    printf("keycode: %d\n", convert_scancode_to_keycode(kdev->fd, scancode));
    tok = strtok(NULL, " ");
  }

  return 1;
}
