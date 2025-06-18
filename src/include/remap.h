#ifndef REMAP_H
#define REMAP_H

#include <croskbd.h>
#include <linux/input.h>

void process_key(KeyboardDevice *kdev, UInputDevice *udev,
                 struct input_event *ev);
void generate_remaps(KeyboardDevice *kdev);

#endif
