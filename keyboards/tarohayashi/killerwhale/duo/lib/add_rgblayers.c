// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "lib/common_killerwhale.h"

// レイヤーごとの発光場所、発光色の設定
const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_PURPLE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_PURPLE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_PURPLE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_PURPLE},
        {61, 2,  HSV_BLACK}
);
const rgblight_segment_t PROGMEM nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_YELLOW},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_YELLOW},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_YELLOW},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_YELLOW},
        {61, 2,  HSV_BLACK}
);
const rgblight_segment_t PROGMEM symbols_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_PURPLE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_PURPLE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_PURPLE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_PURPLE},
        {61, 2,  HSV_BLACK}
);
const rgblight_segment_t PROGMEM numbers_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_AZURE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_AZURE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_AZURE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_AZURE},
        {61, 2,  HSV_BLACK}
);

const rgblight_segment_t PROGMEM qud_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_GREEN} );
const rgblight_segment_t PROGMEM qud_2_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 66, HSV_CORAL} );
const rgblight_segment_t PROGMEM nier_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_WHITE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_WHITE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_WHITE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_WHITE},
        {61, 2,  HSV_BLACK}
);

const rgblight_segment_t PROGMEM nier_2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_AZURE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_AZURE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_AZURE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_AZURE},
        {61, 2,  HSV_BLACK}
);

const rgblight_segment_t PROGMEM nier_3_layer[] = RGBLIGHT_LAYER_SEGMENTS(
        {0, 3,   HSV_PURPLE},
        {3, 24,  HSV_BLACK},
        {27, 1,  HSV_PURPLE},
        {28, 2,  HSV_BLACK},
        {33, 3,  HSV_PURPLE},
        {36, 24, HSV_BLACK},
        {60, 1,  HSV_PURPLE},
        {61, 2,  HSV_BLACK}
);

// レイヤーごとの発光方法の指定
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    base_layer,
    nav_layer,
    symbols_layer,
    numbers_layer,
    qud_layer,
    qud_2_layer,
    nier_layer,
    nier_2_layer,
    nier_3_layer
);

// 初期化
void keyboard_post_init_kb(void) {
   rgblight_layers = my_rgb_layers;
   rgblight_set_layer_state(0, true);
}

// 発光条件の設定
layer_state_t layer_state_set_user(layer_state_t state) {
    for (int i = 0; i < 9; i++) {
        rgblight_set_layer_state(i, kw_config.rgb_layer && layer_state_cmp(state, i));
    }
    return state;
}
