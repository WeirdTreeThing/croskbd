#include <stdio.h>
#include <croskbd.h>
#include <evdev.h>

int keyboard_fd = -1;
char keyboard_ev_name[10] = { 0 };

int main(int argc, char** argv) {
	scan_input_devices();

	if (keyboard_fd < 0) {
		printf("Failed to find keyboard device\n");
		return(1);
	}
	printf("keyboard event: %s\n", keyboard_ev_name);

	evloop();

	return 0;
}
