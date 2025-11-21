#pragma once

#include QMK_KEYBOARD_H
#include "quantum/split_common/transactions.h"

// Shared types for RPC communication between master and slave
typedef struct _colors_to_slave_t {
    uint8_t led_count;               // Number of LEDs in this message
    uint8_t led_indices[8];          // LED indices (global addressing)
    uint8_t led_hsv[8][3];           // HSV for each LED
} colors_to_slave_t;

typedef struct _layer_to_slave_t {
    uint8_t layer_idx;
} layer_to_slave_t;

// Animation modes
typedef enum {
    ANIMATION_UNDERGLOW,    // Default: Corner LED animation on timer
    ANIMATION_SEQUENTIAL    // Sequential LED animation on key press
} animation_mode_t;

// Interface functions called from keymap
void update_leds_on_housekeeping(animation_mode_t mode);
void update_leds_on_keypress(animation_mode_t mode, uint8_t row, uint8_t col);
void clear_all_leds(void);  // Clear all LEDs on both halves
