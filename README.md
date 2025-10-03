# croskbd

# THIS IS CURRENTLY ALPHA SOFTWARE. YOU HAVE BEEN WARNED!

## Why?
Due to the strange keyboard layout on Chromebooks, the default experience while not using ChromeOS is not very ideal.
Croskbd will remap the keys to act in a similar way to how they work on ChromeOS.

Another issue croskbd aims to solve is tablet mode on 2 in 1 Chromebooks. Sometimes the keyboard will be disabled, sometimes the touchpad, sometimes neither will be. Croskbd will be able to disable both of them in tablet mode to make the experience much better.

##  Features

- [x] Read events from keyboard
- [x] Send events via uinput
- [x] Key remapping support
- [x] Top row remaps
- [x] Inverted top row remaps
- [ ] Every misc remap (from ChromeOS and cros-keyboard-map)
- [x] Hammer keyboard support
- [ ] Special case devices (eve, atlas, sarien)
- [ ] Tablet mode handling
- [x] Config file
- [ ] Init services

## Comparison to other solution
### keyd (with cros-keyboard-map)
keyd has much more complex remapping support for those who want to take advantage of it, while croskbd is easier to set up and use. keyd also can't disable the keyboard in tablet mode without hacks, and can't disable the touchpad at all.

### Chromebook xkb keyboard model
The xkb model works OOTB in many setups, except for gnome wayland for some reason. However, it does not work very well. It assumes you have the classic Chromebook layout, which is not true for anything released in the last 3 years, or any special cases. It does not remap every top row key, such as the task switcher button. It does not allow you to use a mod key to get regular fn keys back. And it does not work on devices with a vivaldi keyboard.
