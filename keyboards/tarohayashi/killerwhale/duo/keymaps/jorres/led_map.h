#pragma once

#include QMK_KEYBOARD_H

// Matrix to LED mapping based on physical layout
// Format: matrix_to_led[row][col] = led_index (0-65)
// 255 = no LED at this position
//
// LEDs 0-32: Left half
// LEDs 33-65: Right half
extern const uint8_t PROGMEM matrix_to_led[14][7];
