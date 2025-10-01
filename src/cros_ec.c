#include <fcntl.h>
#include <string.h>
#include <utils.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdlib.h>

struct cros_ec_command {
	uint32_t version;
	uint32_t command;
	uint32_t outsize;
	uint32_t insize;
	uint32_t result;
	uint8_t data[0];
};

#define CROS_EC_DEV_IOC 0xEC
#define CROS_EC_DEV_IOCXCMD _IOWR(CROS_EC_DEV_IOC, 0, struct cros_ec_command)

int ec_dev_init() {
  char version[80] = { 0 };

  int fd = open("/dev/cros_ec", O_RDWR);
  if (fd < 0) {
    err("Failed to open ec device: %s", strerror(errno));
    return -1;
  }

  int ret = read(fd, version, sizeof(version) - 1);
  if (ret <= 0) {
    err("Failed to read ec version: %s", strerror(errno));
    close(fd);
    return -1;
  }
  dbg("cros_ec version: %s", version);

  return fd;
}

int ec_command(int fd, int command, int version, void* outdata, int outsize, void* indata, int insize) {
  int data_size = insize >= outsize ? insize : outsize;
  struct cros_ec_command *cmd = malloc(sizeof(struct cros_ec_command) + data_size);

  cmd->command = command;
  cmd->version = version;
  cmd->result = 0xff;
  cmd->outsize = outsize;
  cmd->insize = insize;
  memcpy(cmd->data, outdata, outsize);

  int ret = ioctl(fd, CROS_EC_DEV_IOCXCMD, cmd);

  if (ret < 0) {
    err("ec command ioctl failed: %s", strerror(errno));
    free(cmd);
    return ret;
  }

  memcpy(indata, cmd->data, insize);
  free(cmd);
  return ret;
}
