#ifndef STATIC_REMAPS_H
#define STATIC_REMAPS_H

#include <croskbd.h>
#include <linux/input-event-codes.h>

// alt + backspace = KEY_DELETE
KeyRemap alt_backspace_remap = {
    .original_key = KEY_BACKSPACE,
    .remap_key = KEY_DELETE,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
};

// ctrl + f5 = printscreen
KeyRemap ctrl_f5_remap = {
    .original_key = KEY_F5,
    .remap_key = KEY_SYSRQ,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTCTRL},
};

#endif
