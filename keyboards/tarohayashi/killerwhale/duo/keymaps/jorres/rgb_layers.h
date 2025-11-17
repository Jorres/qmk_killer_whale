#pragma once

#include QMK_KEYBOARD_H
#include <stdlib.h>

// Animation timing
#define BASE_ANIM_INTERVAL 300  // Change color every 500ms

// LED configuration
#define CORNER_LED_COUNT 8

// State variables (extern declarations)
extern bool rgb_animation_active;
extern uint16_t base_anim_timer;

// Functions to be called from keymap.c
void rgb_layers_init(void);
void rgb_layers_scan(uint8_t base_layer_index);
bool rgb_layers_task(uint8_t base_layer_index);
void apply_layer_lighting(layer_state_t state, uint8_t base_layer_index);
void apply_static_layer_colors(uint8_t layer_idx);
void animate_base_layer(void);
void get_corner_led_hsv(uint8_t corner_idx, uint8_t* h, uint8_t* s, uint8_t* v);
