#include <croskbd.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

  printf("physmap: %s\n", function_row_physmap);

  return 1;
}
