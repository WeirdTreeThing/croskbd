#ifndef PIXEL_REMAPS_H
#define PIXEL_REMAPS_H

#include <croskbd.h>
#include <linux/input-event-codes.h>

KeyRemap pixel_top_row[] = {
	{
		.original_key = KEY_F1,
		.remap_key = KEY_BACK,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F2,
		.remap_key = KEY_REFRESH,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F3,
		.remap_key = KEY_FULL_SCREEN,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F4,
		.remap_key = KEY_SCALE,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F5,
		.remap_key = KEY_BRIGHTNESSDOWN,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F6,
		.remap_key = KEY_BRIGHTNESSUP,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F7,
		.remap_key = KEY_PLAYPAUSE,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F8,
		.remap_key = KEY_MUTE,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F9,
		.remap_key = KEY_VOLUMEDOWN,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{
		.original_key = KEY_F10,
		.remap_key = KEY_VOLUMEUP,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{ },
};

// Pixel inverted top row
KeyRemap pixel_top_row_inverted[] = {
	{
		.original_key = KEY_F1,
		.remap_key = KEY_BACK,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F2,
		.remap_key = KEY_REFRESH,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F3,
		.remap_key = KEY_FULL_SCREEN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F4,
		.remap_key = KEY_SCALE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F5,
		.remap_key = KEY_BRIGHTNESSDOWN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F6,
		.remap_key = KEY_BRIGHTNESSUP,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F7,
		.remap_key = KEY_PLAYPAUSE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F8,
		.remap_key = KEY_MUTE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F9,
		.remap_key = KEY_VOLUMEDOWN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{
		.original_key = KEY_F10,
		.remap_key = KEY_VOLUMEUP,
		.num_mod_keys = 1,
		.mod_keys = {KEY_ASSISTANT},
	},
	{ },
};

// change the assistant thing to a scancode remap in the remap handler
// add the keyboard backlight remaps here since they differ from the standard ones
// add a new config to allow pixel caps lock and if so disable the normal capslock remap

// Pixel misc remaps
KeyRemap pixel_misc_remaps[] = {
	{ // Keyboard backlight down
		.original_key = KEY_F5,
		.remap_key = KEY_KBDILLUMDOWN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_LEFTALT},
	},
	{ // Keyboard backlight up
		.original_key = KEY_F6,
		.remap_key = KEY_KBDILLUMUP,
		.num_mod_keys = 1,
		.mod_keys = {KEY_LEFTALT},
	},
	{ // Map the "menu" key to delete
		.original_key = KEY_F13,
		.remap_key = KEY_DELETE,
		.num_mod_keys = 0,
		.mod_keys = {0},
	},
	{ },
};

#endif
