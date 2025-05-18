#ifndef EVDEV_H
#define EVDEV_H

void scan_input_devices(KeyboardDevice *kdev, TabletSwitchDevice *tdev);
void evloop(KeyboardDevice *kdev);

#endif
