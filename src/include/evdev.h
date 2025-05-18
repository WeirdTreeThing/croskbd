#ifndef EVDEV_H
#define EVDEV_H

#include <croskbd.h>

void scan_input_devices(KeyboardDevice *kdev, TabletSwitchDevice *tdev);
void evloop(KeyboardDevice *kdev);

#endif
