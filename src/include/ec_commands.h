#ifndef EC_COMMANDS_H
#define EC_COMMANDS_H

#include <stdint.h>

#define EC_CMD_HELLO 0x1

struct ec_params_hello {
  uint32_t in_data;
} __attribute__((packed));

struct ec_response_hello {
  uint32_t out_data;
} __attribute__((packed));

#endif
