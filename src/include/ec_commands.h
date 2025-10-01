#ifndef EC_COMMANDS_H
#define EC_COMMANDS_H

#include <croskbd.h>
#include <stdint.h>
#include <utils.h>

#define EC_CMD_HELLO 0x1

struct ec_params_hello {
  uint32_t in_data;
} __attribute__((packed));

struct ec_response_hello {
  uint32_t out_data;
} __attribute__((packed));

#define EC_CMD_GET_KEYBD_CONFIG 0x12A

enum action_key {
	TK_ABSENT = 0,
	TK_BACK = 1,
	TK_FORWARD = 2,
	TK_REFRESH = 3,
	TK_FULLSCREEN = 4,
	TK_OVERVIEW = 5,
	TK_BRIGHTNESS_DOWN = 6,
	TK_BRIGHTNESS_UP = 7,
	TK_VOL_MUTE = 8,
	TK_VOL_DOWN = 9,
	TK_VOL_UP = 10,
	TK_SNAPSHOT = 11,
	TK_PRIVACY_SCRN_TOGGLE = 12,
	TK_KBD_BKLIGHT_DOWN = 13,
	TK_KBD_BKLIGHT_UP = 14,
	TK_PLAY_PAUSE = 15,
	TK_NEXT_TRACK = 16,
	TK_PREV_TRACK = 17,
	TK_KBD_BKLIGHT_TOGGLE = 18,
	TK_MICMUTE = 19,
	TK_MENU = 20,
	TK_DICTATE = 21,
	TK_ACCESSIBILITY = 22,
	TK_DONOTDISTURB = 23,

	TK_COUNT
};

#define KEYBD_CAP_FUNCTION_KEYS BIT(0)
#define KEYBD_CAP_NUMERIC_KEYPAD BIT(1)
#define KEYBD_CAP_SCRNLOCK_KEY BIT(2)
#define KEYBD_CAP_ASSISTANT_KEY BIT(3)

struct ec_response_keybd_config {
  uint8_t num_top_row_keys;
  uint8_t action_keys[MAX_TOP_ROW_KEYS];
  uint8_t capabilities;
} __attribute__((packed));

#endif
