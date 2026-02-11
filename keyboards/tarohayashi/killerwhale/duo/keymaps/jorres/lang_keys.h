#pragma once

#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// Returns true if keycode is in the language-aware range and was handled.
bool process_lang_key(uint16_t keycode, keyrecord_t *record);
