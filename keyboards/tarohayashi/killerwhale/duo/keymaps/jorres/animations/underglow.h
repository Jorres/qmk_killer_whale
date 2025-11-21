#pragma once

#include QMK_KEYBOARD_H

// Underglow animation callbacks
void underglow_on_housekeeping(void);
void underglow_on_keypress(uint8_t row, uint8_t col);
