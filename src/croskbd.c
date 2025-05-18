#include <stdio.h>
#include <croskbd.h>
#include <evdev.h>
#include <uinput.h>

int keyboard_fd = -1;
int uinput_fd = 0;
char keyboard_ev_name[10] = { 0 };

int main(int argc, char** argv) {
	scan_input_devices();

	if (keyboard_fd < 0) {
		printf("Failed to find keyboard device\n");
		return(1);
	}

	uinput_fd = uinput_init();

	if (uinput_fd < 0) {
		printf("Failed to create virtual keyboard\n");
		return 1;
	}

	evloop();

	return 0;
}
