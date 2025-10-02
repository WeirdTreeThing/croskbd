#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utils.h>

struct cros_ec_command_header {
	uint32_t version;
	uint32_t command;
	uint32_t outsize;
	uint32_t insize;
	uint32_t result;
};

#define CROS_EC_DEV_IOC 0xEC
#define CROS_EC_DEV_IOCXCMD                                                    \
	_IOWR(CROS_EC_DEV_IOC, 0, struct cros_ec_command_header)

int ec_dev_init() {
	char version[80] = {0};

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

int ec_command(int fd, int command, int version, void *outdata, int outsize,
			   void *indata, int insize) {
	int data_size = insize >= outsize ? insize : outsize;
	uint8_t *data = malloc(sizeof(struct cros_ec_command_header) + data_size);

	struct cros_ec_command_header header = {.command = command,
											.version = version,
											.result = 0xff,
											.outsize = outsize,
											.insize = insize};

	memcpy(data, &header, sizeof(header));
	if (outsize > 0)
		memcpy(data + sizeof(struct cros_ec_command_header), outdata,
			   data_size);

	int ret = ioctl(fd, CROS_EC_DEV_IOCXCMD, data);

	if (ret < 0) {
		err("ec command ioctl failed: %s", strerror(errno));
		free(data);
		return ret;
	}

	if (insize > 0)
		memcpy(indata, data + sizeof(struct cros_ec_command_header), insize);
	free(data);
	return ret;
}
