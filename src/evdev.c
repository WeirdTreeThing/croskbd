#include <croskbd.h>
#include <dirent.h>
#include <errno.h>
#include <evdev.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utils.h>
#include <vivaldi.h>

static int check_input_dev_name(input_device *dev, char *event,
                                char *match_name) {
  char path[25] = {0};
  snprintf(path, 24, "/dev/input/%s", event);

  int fd = open(path, O_RDONLY);
  if (!fd) {
    err("Failed to read input event: %s", strerror(errno));
    return 0;
  }
  char name[256] = {0};
  if (!ioctl(fd, EVIOCGNAME(sizeof(name)), name))
    err("Failed to get input name: %s", strerror(errno));

  if (!strcmp(name, match_name)) {
    dev->fd = fd;
    snprintf(dev->event_name, 10, "%s", event);
    return 1;
  } else {
    close(fd);
    return 0;
  }
}

static int check_input_dev_id(input_device *dev, char *event, int pid,
                              int vid) {
  char path[25] = {0};
  snprintf(path, 24, "/dev/input/%s", event);

  int fd = open(path, O_RDONLY);
  if (!fd) {
    err("Failed to read input event: %s", strerror(errno));
    return 0;
  }

  char name[256] = {0};
  struct input_id id = {};
  if (!ioctl(fd, EVIOCGNAME(sizeof(name)), name))
    err("Failed to get input name: %s", strerror(errno));
  if (ioctl(fd, EVIOCGID, &id))
    err("Failed to get input id: %s", strerror(errno));

  if (id.vendor == vid && id.product == pid) {
    dev->fd = fd;
    snprintf(dev->event_name, 10, "%s", event);
    return 1;
  } else {
    close(fd);
    return 0;
  }
}

// Returns 1 on success and writes the values to the dev struct
// Returns 0 if no device could be found
static int scan_input_devices(input_device *dev, char *name, int vid, int pid) {
  DIR *directory;
  struct dirent *entry;
  int success = 0;

  directory = opendir("/dev/input/");
  if (!directory) {
    err("Failed to read /dev/input/: %s", strerror(errno));
    exit(1);
  }
  while ((entry = readdir(directory))) {
    if (entry->d_name[0] == 'e') {
      // Shouldn't be possible to find more than one input device here
      if (name != NULL)
        success += check_input_dev_name(dev, entry->d_name, name);
      else
        success += check_input_dev_id(dev, entry->d_name, pid, vid);
    }
  }
  closedir(directory);
  return success > 0;
}

int get_keyboards(input_device *dev) {
  // Fixed name keyboards
  if (scan_input_devices(dev, "AT Translated Set 2 keyboard", 0, 0))
    return 1;
  if (scan_input_devices(dev, "cros_ec", 0, 0))
    return 1;

  // Various hammer keyboards
  int hammer_ids[] = {0x503c, 0x5050, 0x504c, 0x5052, 0x5057, 0x505b,
                      0x5030, 0x503d, 0x5044, 0x5061, 0x502b};
  for (int i = 0; i < (sizeof(hammer_ids) / sizeof(hammer_ids[0])); i++)
    if (scan_input_devices(dev, NULL, 0x18d1, hammer_ids[i]))
      return 1;

  return 0;
}

void close_dev_fds(KeyboardDevice *kdev, TabletSwitchDevice *tdev) {
  if (kdev->fd > 0)
    close(kdev->fd);
  if (tdev->fd > 0)
    close(tdev->fd);
}

void load_kb_layout_data(KeyboardDevice *kdev) {
  if (!load_kb_vivaldi_data(kdev)) {
    dbg("Top row layout data not found, using default layout.");
    kdev->num_top_row_keys = 10;
    int default_top_row[] = {158, 159, 173, 372, 120, 224, 225, 113, 114, 115};
    memcpy(kdev->top_row_keys, default_top_row, sizeof(default_top_row));
  }
}
