#ifndef UINPUT_H
#define UINPUT_H

#include <croskbd.h>

void uinput_send_event(KeyboardDevice *kdev, UInputDevice *udev, int type, int code, int value);
void uinput_teardown(UInputDevice *udev);
void uinput_init(UInputDevice *udev);

#endif
