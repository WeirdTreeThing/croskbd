#ifndef REMAP_H
#define REMAP_H

#include <croskbd.h>

void generate_remaps(KeyboardDevice *kdev);
int remap_key(KeyboardDevice *kdev, int key);
void add_mod_key(int key);
void remove_mod_key(int key);

#endif
