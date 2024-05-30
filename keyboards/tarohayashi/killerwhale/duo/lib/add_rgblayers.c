// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "lib/common_killerwhale.h"

// レイヤーごとの発光場所、発光色の設定
const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS( { 0, 66, HSV_YELLOW} );
const rgblight_segment_t PROGMEM arrows_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_GREEN} );
const rgblight_segment_t PROGMEM mouse_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_AZURE} );
const rgblight_segment_t PROGMEM nav_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_GOLDENROD} );
const rgblight_segment_t PROGMEM symbols_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_PURPLE} );
const rgblight_segment_t PROGMEM numbers_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_MAGENTA} );

// レイヤーごとの発光方法の指定
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    base_layer,
    arrows_layer,
    mouse_layer,
    nav_layer,
    symbols_layer,
    numbers_layer
);


// 初期化
void keyboard_post_init_kb(void) {
    rgblight_layers = my_rgb_layers;
}

// 発光条件の設定
layer_state_t layer_state_set_user(layer_state_t state) {
    for (int i = 0; i < 6; i++) {
        rgblight_set_layer_state(i, kw_config.rgb_layer && layer_state_cmp(state, i));
    }
    return state;
}
