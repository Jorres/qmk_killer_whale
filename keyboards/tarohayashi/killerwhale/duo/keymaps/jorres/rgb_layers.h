#pragma once

#include QMK_KEYBOARD_H
#include <stdlib.h>

// Animation timing
#define BASE_ANIM_INTERVAL 500  // Change color every 500ms

// LED configuration
#define CORNER_LED_COUNT_PER_HALF 4

// State variables (extern declarations)
extern bool rgb_animation_active;
extern uint16_t base_anim_timer;

// Functions to be called from keymap.c
void rgb_layers_init(void);
void rgb_layers_task(uint8_t base_layer_index);
void rgb_layers_scan(uint8_t base_layer_index);
void apply_layer_lighting(layer_state_t state, uint8_t base_layer_index);
void animate_base_layer(void);
