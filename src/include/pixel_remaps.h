#ifndef PIXEL_REMAPS_H
#define PIXEL_REMAPS_H

#include <croskbd.h>
#include <linux/input-event-codes.h>

const KeyRemap pixel_top_row[] = {
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
const KeyRemap pixel_top_row_inverted[] = {
	{
		.original_key = KEY_F1,
		.remap_key = KEY_BACK,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F2,
		.remap_key = KEY_REFRESH,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F3,
		.remap_key = KEY_FULL_SCREEN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F4,
		.remap_key = KEY_SCALE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F5,
		.remap_key = KEY_BRIGHTNESSDOWN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F6,
		.remap_key = KEY_BRIGHTNESSUP,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F7,
		.remap_key = KEY_PLAYPAUSE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F8,
		.remap_key = KEY_MUTE,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F9,
		.remap_key = KEY_VOLUMEDOWN,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{
		.original_key = KEY_F10,
		.remap_key = KEY_VOLUMEUP,
		.num_mod_keys = 1,
		.mod_keys = {KEY_EVE_ASSISTANT},
	},
	{ },
};

// Pixel misc remaps
const KeyRemap pixel_misc_remaps[] = {
	{ // Map the assistant key to super
		.original_key = KEY_EVE_ASSISTANT,
		.remap_key = KEY_LEFTMETA,
		.num_mod_keys = 0,
		.mod_keys = {0},
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
