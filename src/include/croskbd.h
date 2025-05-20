#ifndef CROSKBD_H
#define CROSKBD_H

// https://chromium.googlesource.com/chromiumos/platform/ec/+/9c9df9c7d40daf7f77f8b4132aa959ab024b219c/include/ec_commands.h#7101
#define MAX_TOP_ROW_KEYS 15

typedef struct {
  int invert_top_row;
  int handle_tablet_switch;
  int del_key;
} Settings;

typedef struct {
  int fd;
  char ev_name[10];
  int top_row_keys[MAX_TOP_ROW_KEYS];
  int num_top_row_keys;
} KeyboardDevice;

typedef struct {
  int fd;
  int tablet_mode;
} TabletSwitchDevice;

typedef struct {
  int fd;
} UInputDevice;

#endif
