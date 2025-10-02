#ifndef CROSKBD_H
#define CROSKBD_H

// https://chromium.googlesource.com/chromiumos/platform/ec/+/9c9df9c7d40daf7f77f8b4132aa959ab024b219c/include/ec_commands.h#7101
#define MAX_TOP_ROW_KEYS 15
#define MAX_REMAPS 40 // adjust if needed

typedef struct {
  int original_key;
  int remap_key;
  int mod_keys[4];
  int num_mod_keys;
  int repeatable;
} KeyRemap;

typedef struct {
  int invert_top_row;
  int handle_tablet_switch;
  int delete_key;
  int debug;
  int override_key_codes;
} Settings;

typedef struct {
  int fd;
  int ec_fd;
  int kbd_caps;
  char ev_name[10];
  int top_row_keys[MAX_TOP_ROW_KEYS];
  int num_top_row_keys;
  int has_vivaldi;
  KeyRemap remaps[MAX_REMAPS];
  int num_remaps;
} KeyboardDevice;

typedef struct {
  int fd;
  int tablet_mode;
} TabletSwitchDevice;

typedef struct {
  int fd;
} UInputDevice;

#endif
