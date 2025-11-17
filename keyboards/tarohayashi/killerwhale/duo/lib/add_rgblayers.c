// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "lib/common_killerwhale.h"

// NOTE: We no longer use RGBLIGHT_LAYERS as it conflicts with direct LED control.
// All RGB logic is now handled in keymap.c using rgblight_sethsv_range/at functions.
//
// extern bool rgb_animation_active;
// extern uint16_t base_anim_timer;

// // External function from keymap.c for applying layer colors
// extern void init_base_layer_animation(void);

// 初期化 - empty, RGB is initialized in keymap.c
void keyboard_post_init_kb(void) {
}

// External function from keymap.c for applying layer colors
extern void apply_layer_lighting(layer_state_t state);

// 発光条件の設定 - delegate to keymap.c
layer_state_t layer_state_set_user(layer_state_t state) {
    apply_layer_lighting(state);
    return state;
}
