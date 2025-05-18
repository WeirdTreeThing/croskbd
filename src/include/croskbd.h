#ifndef CROSKBD_H
#define CROSKBD_H

typedef struct {
	int invert_top_row;
	int handle_tablet_switch;
	int del_key;
} Settings;

typedef struct {
	int fd;
	char ev_name[10];
} KeyboardDevice;

typedef struct {
	int fd;
	int tablet_mode;
} TabletSwitchDevice;

typedef struct {
	int fd;
} UInputDevice;

#endif
