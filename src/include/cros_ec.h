#ifndef CROS_EC_H
#define CROS_EC_H

int ec_dev_init();
int ec_command(int fd, int command, int version, void* outdata, int outsize, void* indata, int insize);

#endif
