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
        printf("Failed to open %s\n", path);
        return;
    }
    char name[256] = { 0 };
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
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
        printf("failed to open /dev/input/");
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
