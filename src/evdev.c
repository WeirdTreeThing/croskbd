#include <croskbd.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

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
  } else if (!strcmp(name, "Tablet Mode Switch")) {
    tdev->fd = fd;
    return;
  } else {
    close(fd);
    return;
  }
}

void scan_input_devices(KeyboardDevice *kdev, TabletSwitchDevice *tdev) {
  DIR *directory;
  struct dirent *entry;

  directory = opendir("/dev/input/");
  if (!directory) {
    perror("Failed to read /dev/input/");
    return;
  }
  while ((entry = readdir(directory))) {
    if (entry->d_name[0] == 'e') {
      check_input_dev(entry->d_name, kdev, tdev);
    }
  }
  closedir(directory);
}

void evloop(KeyboardDevice *kdev) {
  struct input_event ev;
  fd_set rdfs;
  FD_ZERO(&rdfs);
  FD_SET(kdev->fd, &rdfs);

  while (1) {
    select(kdev->fd + 1, &rdfs, NULL, NULL, NULL);
    read(kdev->fd, &ev, sizeof(struct input_event));
    printf("event: ev.value=%d ev.code=%d\n", ev.value, ev.code);
  }
}
