#ifndef STATIC_REMAPS_H
#define STATIC_REMAPS_H

#include <croskbd.h>
#include <linux/input-event-codes.h>

// Generic

// alt + backspace = KEY_DELETE
KeyRemap alt_backspace_remap = {
    .original_key = KEY_BACKSPACE,
    .remap_key = KEY_DELETE,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

// Non-vivaldi

// ctrl + f5 = printscreen
KeyRemap ctrl_f5_remap = {
    .original_key = KEY_F5,
    .remap_key = KEY_SYSRQ,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTCTRL},
};

// alt + f6 = kb light down
KeyRemap alt_f6_remap = {
    .original_key = KEY_F6,
    .remap_key = KEY_KBDILLUMDOWN,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

// alt + f7 = kb light up
KeyRemap alt_f7_remap = {
    .original_key = KEY_F7,
    .remap_key = KEY_KBDILLUMUP,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

// Vivaldi

// ctrl + scale = printscreen
KeyRemap ctrl_scale_remap = {
    .original_key = KEY_SCALE,
    .remap_key = KEY_SYSRQ,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTCTRL},
};

// alt + bl down = kb light down
KeyRemap alt_bldown_remap = {
    .original_key = KEY_BRIGHTNESSDOWN,
    .remap_key = KEY_KBDILLUMDOWN,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

// alt + bl up = kb light up
KeyRemap alt_blup_remap = {
    .original_key = KEY_BRIGHTNESSUP,
    .remap_key = KEY_KBDILLUMUP,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

#endif
