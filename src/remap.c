#include <croskbd.h>
#include <ec_commands.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <pixel_remaps.h>
#include <static_remaps.h>
#include <stdlib.h>
#include <string.h>
#include <uinput.h>
#include <unistd.h>
#include <utils.h>

#define MAX_MOD_KEYS 10

#define is_modkey(k) (k == KEY_LEFTCTRL || \
k == KEY_LEFTALT || \
k == KEY_LEFTSHIFT || \
k == KEY_LEFTMETA || \
k == KEY_RIGHTCTRL || \
k == KEY_RIGHTALT || \
k == KEY_RIGHTSHIFT || \
k == KEY_RIGHTMETA || \
k == KEY_FN || \
k == KEY_ASSISTANT )

extern Settings settings;

static int mod_key_codes[MAX_MOD_KEYS] = {0};
static int mod_keys_pressed = 0;

static void add_mod_key(int key) {
	if (mod_keys_pressed == MAX_MOD_KEYS) {
		err("Max mod keys reached");
		return;
	}
	mod_key_codes[mod_keys_pressed] = key;
	mod_keys_pressed++;
}

static void remove_mod_key(int key) {
	if (mod_keys_pressed == 0) {
		err("Min mod keys reached");
		return;
	}

	for (int i = 0; i < mod_keys_pressed; i++) {
		if (mod_key_codes[i] == key)
			mod_key_codes[i] = 0;
		if (i + 1 == MAX_MOD_KEYS)
			break;
		if (mod_key_codes[i] == 0 && mod_key_codes[i + 1] != 0) {
			mod_key_codes[i] = mod_key_codes[i + 1];
			mod_key_codes[i + 1] = 0;
		}
		if (mod_key_codes[i] == 0 && mod_key_codes[i + 1] == 0)
			break;
	}

	mod_keys_pressed--;
}

static int check_mod_keys(KeyboardDevice *kdev, int remap) {
	// returns 1 if they match, 0 if not
	if (mod_keys_pressed != kdev->remaps[remap].num_mod_keys)
		return 0;
	for (int i = 0; i < kdev->remaps[remap].num_mod_keys; i++) {
		if (mod_key_codes[i] != kdev->remaps[remap].mod_keys[i])
			return 0;
	}
	return 1;
}

static int exact_remap(KeyboardDevice *kdev, int key) {
	// Returns remap index if exists, otherwise returns -1

	for (int i = 0; i < kdev->num_remaps; i++) {
		if (kdev->remaps[i].original_key != key)
			continue;
		if (check_mod_keys(kdev, i))
			return i;
	}
	return -1;
}

static int repeatable_remap(KeyboardDevice *kdev, int key) {
	// Returns index if there exists a remap with the same og keycode and
	// repeatable set to 1 Even if the remap keycode is the same across
	// multiple, there *should* only be one that can be repeatable at least i
	// think and otherwise returns -1

	for (int i = 0; i < kdev->num_remaps; i++) {
		if (kdev->remaps[i].original_key == key && kdev->remaps[i].repeatable)
			return i;
	}
	return -1;
}

static void send_mod_key_events(KeyboardDevice *kdev, UInputDevice *udev,
								int remap, int state) {
	if (kdev->remaps[remap].preserve_mod_keys)
		return;
	for (int i = 0; i < kdev->remaps[remap].num_mod_keys; i++)
		uinput_send_event(kdev, udev, EV_KEY, kdev->remaps[remap].mod_keys[i],
						  state);
}

// return 1 on success, 0 on faliure
static int add_remap(KeyboardDevice *kdev, KeyRemap *remap) {
	if (kdev->num_remaps + 1 >= MAX_REMAPS) {
		err("remap list full bruh %d", kdev->num_remaps);
		return 0;
	}
	// i tried to use a memcpy here and it didnt work and idk why and i didnt
	// feel like figuring it out
	kdev->remaps[kdev->num_remaps].original_key = remap->original_key;
	kdev->remaps[kdev->num_remaps].remap_key = remap->remap_key;
	kdev->remaps[kdev->num_remaps].num_mod_keys = remap->num_mod_keys;
	kdev->remaps[kdev->num_remaps].preserve_mod_keys = remap->preserve_mod_keys;
	kdev->remaps[kdev->num_remaps].repeatable = 0;
	for (int i = 0; i < remap->num_mod_keys; i++) {
		kdev->remaps[kdev->num_remaps].mod_keys[i] = remap->mod_keys[i];
	}
	kdev->num_remaps++;
	return 1;
}

static int add_remap_array(KeyboardDevice *kdev, KeyRemap remaps[]) {
	int ret;
	for (; remaps->original_key; remaps++) {
		ret = add_remap(kdev, remaps);
		if (!ret)
			return ret;
	}

	return 1;
}

static void generate_top_row_remaps(KeyboardDevice *kdev) {
	for (int i = 0; i < kdev->num_top_row_keys; i++) {
		int original_key;
		int primary_remap_key;
		int secondary_remap_key;

		if (settings.invert_top_row) {
			primary_remap_key = KEY_F1 + i;
			secondary_remap_key = kdev->top_row_keys[i];
		} else {
			primary_remap_key = kdev->top_row_keys[i];
			secondary_remap_key = KEY_F1 + i;
		}
		if (kdev->has_vivaldi)
			original_key = kdev->top_row_keys[i];
		else
			original_key = KEY_F1 + i;

		int remap_mod_key = KEY_LEFTMETA;
		if (kdev->kbd_caps & KEYBD_CAP_FUNCTION_KEYS)
			remap_mod_key = KEY_FN;

		KeyRemap remap_primary = {
			.original_key = original_key,
			.remap_key = primary_remap_key,
			.num_mod_keys = 0,
			.mod_keys = {0},
		};

		KeyRemap remap_secondary = {
			.original_key = original_key,
			.remap_key = secondary_remap_key,
			.num_mod_keys = 1,
			.mod_keys = {remap_mod_key},
		};

		if (settings.vt_switch_keymaps) {
			KeyRemap remap_vt_switch = {
				.original_key = original_key,
				.remap_key = KEY_F1 + i,
				.num_mod_keys = 2,
				.mod_keys = {KEY_LEFTCTRL, KEY_LEFTALT},
				.preserve_mod_keys = 1,
			};
			add_remap(kdev, &remap_vt_switch);
		}

		add_remap(kdev, &remap_primary);
		add_remap(kdev, &remap_secondary);
	}
}

void process_key(KeyboardDevice *kdev, UInputDevice *udev,
				 struct input_event *ev) {
	// original keycode will always live in ev
	// keycode will either be remapped or not
	int keycode = ev->code;

	if (ev->type != EV_KEY)
		return;

	int remap = exact_remap(kdev, keycode);

	switch (ev->value) {
	case 0:
		if (remap != -1) {
			send_mod_key_events(kdev, udev, remap, 0);
			keycode = kdev->remaps[remap].remap_key;
		} else {
			int rep_remap = repeatable_remap(kdev, ev->code);
			if (rep_remap != -1) {
				kdev->remaps[rep_remap].repeatable = 0;
				keycode = kdev->remaps[rep_remap].remap_key;
			}
		}
		uinput_send_event(kdev, udev, EV_KEY, keycode, 0);
		if (is_modkey(ev->code))
			remove_mod_key(ev->code);
		break;
	case 1:
		if (remap != -1) {
			keycode = kdev->remaps[remap].remap_key;
			send_mod_key_events(kdev, udev, remap, 0);
		}
		uinput_send_event(kdev, udev, EV_KEY, keycode, 1);
		if (is_modkey(ev->code))
			add_mod_key(ev->code);
		break;
	case 2:
		if (remap != -1) {
			kdev->remaps[remap].repeatable = 1;
			send_mod_key_events(kdev, udev, remap, 0);
			keycode = kdev->remaps[remap].remap_key;
		} else {
			int rep_remap = repeatable_remap(kdev, ev->code);
			if (rep_remap != -1)
				keycode = kdev->remaps[rep_remap].remap_key;
		}
		uinput_send_event(kdev, udev, EV_KEY, keycode, 2);
		break;
	default:
		err("Invalid ev.value");
		break;
	}

	uinput_send_event(kdev, udev, EV_SYN, SYN_REPORT, 0); // Sync
}

void pixel_remap_config(KeyboardDevice *kdev) {
	// Assign KEY_ASSISTANT to the scancode 0xd8
	int ret;
	struct input_keymap_entry ke;
	ke.scancode[0] = 0xd8;
	ke.len = 1;
	ke.keycode = KEY_ASSISTANT;

	ret = ioctl(kdev->fd, EVIOCSKEYCODE_V2, &ke);
	if (ret < 0)
		err("EVIOCSKEYCODE_V2 failed: %s", strerror(ret));

	// Add vt switch
	if (settings.invert_top_row)
		add_remap_array(kdev, pixel_top_row_inverted);
	else
		add_remap_array(kdev, pixel_top_row);
	add_remap_array(kdev, pixel_misc_remaps);
}

void dell_remap_config();
void generic_remap_config();

void add_remaps(KeyboardDevice *kdev) {
	char *pn = read_to_string("/sys/class/dmi/id/product_name");
	if (pn != NULL) {
		// clean up string
		for (int i = 0; i < strlen(pn); i++)
			if (pn[i] == '\n')
				pn[i] = 0;

		if (!strcmp("Eve", pn)) {
			warn("Eve");
			pixel_remap_config(kdev);
		} else if (!strcmp("Atlas", pn)) {
			warn("Atlas");
		} else if (!strcmp("Nocturne", pn)) {
			warn("Nocturne");
		} else if (!strcmp("Drallion", pn)) {
			warn("Drallion");
		} else if (!strcmp("Sarien", pn)) {
			warn("Sarien");
		} else if (!strcmp("Arcada", pn)) {
			warn("Arcada");
		} else {
			generate_top_row_remaps(kdev);
			add_remap_array(kdev, pixel_misc_remaps);
		}

		free(pn);
	}

	if (settings.delete_key)
		add_remap(kdev, &alt_backspace_remap);

	// Convert these to a remap array
	if (kdev->has_vivaldi) {
		add_remap(kdev, &ctrl_scale_remap);
		add_remap(kdev, &alt_bldown_remap);
		add_remap(kdev, &alt_blup_remap);
	} else {
		add_remap(kdev, &ctrl_f5_remap);
		add_remap(kdev, &alt_f6_remap);
		add_remap(kdev, &alt_f7_remap);
	}
}
