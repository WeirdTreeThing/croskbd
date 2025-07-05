#ifndef STATIC_REMAPS_H
#define STATIC_REMAPS_H

#include <croskbd.h>
#include <linux/input-event-codes.h>

KeyRemap alt_backspace_remap = {
    .original_key = KEY_BACKSPACE,
    .remap_key = KEY_DELETE,
    .num_mod_keys = 1,
    .mod_keys = {KEY_LEFTALT},
    .repeatable = 0,
};

#endif
