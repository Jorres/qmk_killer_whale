// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "lib/common_killerwhale.h"
#include "print.h"

// NOTE: We no longer use RGBLIGHT_LAYERS as it conflicts with direct LED control.
// All RGB logic is now handled in keymap.c using rgblight_sethsv_range/at functions.
//
// extern bool rgb_animation_active;
// extern uint16_t base_anim_timer;

// // External function from keymap.c for applying layer colors
// extern void init_base_layer_animation(void);

// 初期化 - empty, RGB is initialized in keymap.c
void keyboard_post_init_kb(void) {
    // debug_enable=true;
    // debug_matrix=true;
    keyboard_post_init_user();
}

// Language-aware SYMBOLS layer: switch to US on entry, back to RU on exit
#define SYMBOLS_LAYER 2
#define LANG_SWITCH_DELAY_MS 50

extern uint8_t current_os_layout;
static bool symbols_switched_layout = false;

layer_state_t layer_state_set_user(layer_state_t state) {
    bool symbols_active = layer_state_cmp(state, SYMBOLS_LAYER);

    if (symbols_active && !symbols_switched_layout && current_os_layout == 1) {
        uprintf("SYMBOLS: switching to US\n");
        symbols_switched_layout = true;
        tap_code16(LGUI(KC_SPC));
        wait_ms(LANG_SWITCH_DELAY_MS);
    } else if (!symbols_active && symbols_switched_layout) {
        uprintf("SYMBOLS: switching back to RU\n");
        symbols_switched_layout = false;
        wait_ms(LANG_SWITCH_DELAY_MS);
        tap_code16(LGUI(KC_SPC));
    }

    return state;
}
