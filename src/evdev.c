#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <croskbd.h>

static void check_input_dev(char* event) {
    char path[25] = { 0 };
    snprintf(path, 24, "/dev/input/%s", event);

    int fd = open(path, O_RDONLY);
    if (!fd) {
        perror("Failed to read input event");
        return;
    }
    char name[256] = { 0 };
    if (ioctl(fd, EVIOCGNAME(sizeof(name)), name))
		perror("Failed to get input name");
    //printf("Device name: %s\n", name);

    if (!strcmp(name, "AT Translated Set 2 keyboard")) {
        keyboard_fd = fd;
        snprintf(keyboard_ev_name, 10, "%s", event);
        return;
    }
    else {
        close(fd);
        return;
    }
}

void scan_input_devices(void) {
    DIR *directory;
    struct dirent *entry;

    directory = opendir("/dev/input/");
    if (!directory) {
        perror("Failed to read /dev/input/");
        return;
    }
    while ((entry = readdir(directory))) {
        if (entry->d_name[0] == 'e') {
            //printf("event: %s\n", entry->d_name);
            check_input_dev(entry->d_name);
        }
    }
    closedir(directory);
}

void evloop(void) {
	struct input_event ev;
	fd_set rdfs;
	FD_ZERO(&rdfs);
	FD_SET(keyboard_fd, &rdfs);

	while (1) {
		select(keyboard_fd + 1, &rdfs, NULL, NULL, NULL);
		read(keyboard_fd, &ev, sizeof(struct input_event));
		printf("event: ev.value=%d ev.code=%d\n", ev.value, ev.code);
	}
}
