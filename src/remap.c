#include <croskbd.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <stdio.h>
#include <uinput.h>

#define MAX_MOD_KEYS 5
/*
static int mod_key_codes[MAX_MOD_KEYS] = {0};
static int mod_keys_pressed = 0;


listatic int check_mod_key(int key) {
  // return value 0 means key is not in list
  // 1 means key is in list

  for (int i = 0; i < mod_keys_pressed; i++) {
    if (mod_keys[i] == key)
      return 1;
  }

  return 0;
};


static void add_mod_key(int key) {
  if ((mod_keys_pressed + 1) == MAX_MOD_KEYS) {
    printf("Max mod keys reached\n");
    return;
  }
  mod_key_codes[mod_keys_pressed] = key;
  mod_keys_pressed++;
};

static void remove_mod_key(int key) {
  if ((mod_keys_pressed - 1) == 0) {
    printf("Min mod keys reached\n");
    return;
  }
  mod_keys_pressed--;
  mod_key_codes[mod_keys_pressed] = 0;
};
*/
static int remap_key(KeyboardDevice *kdev, int key) {
  // todo: also check for mod keys

  for (int i = 0; i < kdev->num_remaps; i++) {
    if (kdev->remaps[i].original_key == key)
      return kdev->remaps[i].remap_key;
  }

  return key;
}

void process_key(KeyboardDevice *kdev, UInputDevice *udev,
                 struct input_event *ev) {
  int keycode = remap_key(kdev, ev->code);
  uinput_send_event(udev, ev->type, keycode, ev->value);
}

void generate_remaps(KeyboardDevice *kdev) {
  // top row map
  for (int i = 0; i < kdev->num_top_row_keys; i++) {
    int original_key = KEY_F1 + i;
    int remap_key = kdev->top_row_keys[i];

    if (kdev->has_vivaldi)
      original_key = kdev->top_row_keys[i];

    kdev->remaps[i].original_key = original_key;
    kdev->remaps[i].remap_key = remap_key;
    kdev->num_remaps++;
  }
}
