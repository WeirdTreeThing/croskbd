#ifndef EVDEV_H
#define EVDEV_H

#include <croskbd.h>

void scan_input_devices(KeyboardDevice *kdev, TabletSwitchDevice *tdev);
void load_kb_layout_data(KeyboardDevice *kdev);

#endif
