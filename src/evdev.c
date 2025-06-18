#include <croskbd.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vivaldi.h>

static void check_input_dev(char *event, KeyboardDevice *kdev,
                            TabletSwitchDevice *tdev) {
  char path[25] = {0};
  snprintf(path, 24, "/dev/input/%s", event);

  int fd = open(path, O_RDONLY);
  if (!fd) {
    perror("Failed to read input event");
    return;
  }
  char name[256] = {0};
  if (!ioctl(fd, EVIOCGNAME(sizeof(name)), name))
    perror("Failed to get input name");

  if (!strcmp(name, "AT Translated Set 2 keyboard")) {
    kdev->fd = fd;
    snprintf(kdev->ev_name, 10, "%s", event);
    return;
  } else if (!strcmp(name, "cros_ec")) {
    kdev->fd = fd;
    snprintf(kdev->ev_name, 10, "%s", event);
    return;
  } else if (!strcmp(name, "Tablet Mode Switch")) {
    tdev->fd = fd;
    return;
  } else {
    close(fd);
    return;
  }
}

void close_dev_fds(KeyboardDevice *kdev, TabletSwitchDevice *tdev) {
  if (kdev->fd > 0)
    close(kdev->fd);
  if (tdev->fd > 0)
    close(tdev->fd);
}

void load_kb_layout_data(KeyboardDevice *kdev) {
  if (!load_kb_vivaldi_data(kdev)) {
    printf("Top row layout data not found, using default layout.\n");
    kdev->num_top_row_keys = 10;
    int default_top_row[] = {158, 159, 173, 372, 120, 224, 225, 113, 114, 115};
    memcpy(kdev->top_row_keys, default_top_row, sizeof(default_top_row));
  }
}

void scan_input_devices(KeyboardDevice *kdev, TabletSwitchDevice *tdev) {
  DIR *directory;
  struct dirent *entry;

  directory = opendir("/dev/input/");
  if (!directory) {
    perror("Failed to read /dev/input/");
    exit(1);
  }
  while ((entry = readdir(directory))) {
    if (entry->d_name[0] == 'e') {
      check_input_dev(entry->d_name, kdev, tdev);
    }
  }
  closedir(directory);
}
