#include QMK_KEYBOARD_H
#include "lib/add_keycodes.h"

enum layer_number {
    BASE = 0,
    ARROWS_NUMBERS,
    MOUSE,
    NAVIGATION,
    SYMBOLS,
};

enum custom_keycodes {
    MACRO_CHANGE_LANG = SAFE_RANGE,
    TMUX_1,
    TMUX_2,
    TMUX_3,
    TMUX_4,
    TMUX_5,
    TMUX_6,
    TMUX_7,
    TMUX_8,
    TMUX_9,
    TMUX_10,

    TM_NWIN,

    BRO_LEF,
    BRO_RIG,
    BRO_ADL,
    BRO_NEW,

    NAV_TRM,
    NAV_BRO,
    NAV_TEL,
    NAV_XXX,

    I_ESC,
};

// キーマップの設定
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        // 左手
        KC_TAB,  KC_Q,      KC_ESC,             TO(ARROWS_NUMBERS), _______, _______,
        KC_LCTL, KC_A,      KC_W,               KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,               KC_D, KC_F, KC_G,
                 _______, KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
                            MO(NAVIGATION),
        LT(SYMBOLS, KC_SPC), KC_ENT,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                    _______,

        // 右手
        _______, _______, TO(ARROWS_NUMBERS), _______,          KC_P,    KC_LBRC,
        KC_Y,    KC_U,    I_ESC,              KC_O,             KC_SCLN, KC_QUOT,
        KC_H,    KC_J,    KC_K,               KC_L,             KC_SLSH, KC_RSFT,
        KC_N,    KC_M,    KC_COMM,            KC_DOT,           MACRO_CHANGE_LANG,
                                              MO(NAVIGATION),
        KC_ENT,  LT(SYMBOLS, KC_BSPC),
        KC_UP,   KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                     _______
    ),
    [ARROWS_NUMBERS] = LAYOUT(
        // 左手
        _______, KC_1,    _______, TO(BASE), _______,  _______,
        _______, _______, KC_2,    KC_3,    KC_4,     KC_5,
        _______, _______, _______, _______, _______,  KC_LEFT,
                 _______, _______, _______, _______,  KC_DOWN,
                          _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        _______,  _______, TO(BASE), _______, KC_0,    _______,
        KC_6,     KC_7,    KC_8,    KC_9,    _______, _______,
        KC_RIGHT, _______, _______, _______, _______, _______,
        KC_UP,    _______, _______, _______, _______,
                                   _______,

        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [MOUSE] = LAYOUT(
        // 左手
        _______, _______, _______, TO(BASE), _______,  _______,
        _______, _______, _______, _______,  _______,  _______,
        _______, _______, _______, KC_MS_BTN2, KC_MS_BTN1, MOD_SCRL,
                 _______, _______, _______, _______, _______,
                          MOD_SCRL,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,
        // 右手
        _______, _______, TO(BASE), _______, _______, _______,
        _______, _______, _______,  _______, _______, _______,
        MOD_SCRL, KC_MS_BTN1, KC_MS_BTN2, _______, _______, _______,
        _______, _______, _______, _______, _______,
                                   MOD_SCRL,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [NAVIGATION] = LAYOUT(
        // 左手
        _______, TMUX_1,  KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
        _______, _______, TMUX_2,  TMUX_3,  TMUX_4,  TMUX_5,
        _______, _______, NAV_TRM, NAV_BRO, NAV_TEL, NAV_XXX,
                 _______, _______, _______, _______, _______,
                          KC_TRNS,
        BRO_LEF, BRO_NEW,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        _______, _______, _______, _______, TMUX_10, TO(BASE),
        TMUX_6,  TMUX_7,  TMUX_8,  TMUX_9,  _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, TM_NWIN, _______, _______, _______,
                                   KC_TRNS,
        BRO_ADL, BRO_RIG,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [SYMBOLS] = LAYOUT(
        // 左手
        _______, _______, _______,  _______, _______, _______,
        _______, _______, KC_EQUAL, KC_BSLS, KC_PIPE, _______,
        _______, KC_ASTR, KC_GRV,   KC_DLR,  KC_AT,    KC_EXLM,
                 KC_CIRC, KC_MINS,  KC_LABK, KC_RABK, _______,
                          _______,
        _______, _______,
        _______, _______, _______,  _______,          _______,
        _______, _______,                             _______,

        // 右手
        _______, _______, _______, _______, _______, TO(BASE),
        _______, KC_LCBR, KC_RCBR, KC_PLUS, _______, _______,
        KC_QUES, KC_LPRN, KC_RPRN, KC_HASH, KC_AMPR, KC_TILD,
        _______, KC_LBRC, KC_RBRC, KC_UNDS, KC_PERC,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    )
    // [BALL_SETTINGS] = LAYOUT(
    //     // 左手
    //     XXXXXXX,    XXXXXXX, XXXXXXX, QK_USER_14, _______, L_CHMOD,
    //     XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, L_SPD_I, XXXXXXX,
    //     AUTO_MOUSE, XXXXXXX, XXXXXXX, L_ANG_D, L_INV,   L_ANG_I,
    //                 XXXXXXX, XXXXXXX,XXXXXXX, L_SPD_D, XXXXXXX,
    //                          INV_SCRL,
    //     XXXXXXX, XXXXXXX,
    //     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
    //     XXXXXXX, INV_SCRL,                           XXXXXXX,
    //     // 右手
    //     R_CHMOD, _______, QK_USER_14, XXXXXXX, XXXXXXX, XXXXXXX,
    //     XXXXXXX, R_SPD_I,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //     R_ANG_D, R_INV,   R_ANG_I, XXXXXXX, XXXXXXX, AUTO_MOUSE,
    //     XXXXXXX, R_SPD_D,   XXXXXXX, XXXXXXX, XXXXXXX,
    //                                  INV_SCRL,
    //     XXXXXXX, XXXXXXX,
    //     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
    //     XXXXXXX, XXXXXXX,                            XXXXXXX
    // ),
    // [LIGHT_SETTINGS] = LAYOUT(
    //     // 左手
    //     XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_RMOD, _______,
    //     XXXXXXX, RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG,
    //     OLED_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //              RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, XXXXXXX,
    //                       QK_USER_15,
    //     RGB_MOD, RGB_RMOD,
    //     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
    //     XXXXXXX, XXXXXXX,                            XXXXXXX,
    //     // 右手
    //     _______, RGB_MOD, RGB_RMOD, XXXXXXX, XXXXXXX, XXXXXXX,
    //     RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, XXXXXXX,
    //     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OLED_MOD,
    //     XXXXXXX, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD,
    //                                QK_USER_15,
    //     RGB_RMOD, RGB_MOD,
    //     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
    //     XXXXXXX, XXXXXXX,                            XXXXXXX
    // )
};

const uint16_t PROGMEM lgui[] = {KC_Z, KC_S, COMBO_END};
combo_t key_combos[] = {
    COMBO(lgui, KC_LGUI),
};

void keyboard_post_init_user(void) {
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
}

bool oled_task_user(void) {
    oled_clear();
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case BASE:
            oled_write_ln_P(PSTR("BASE"), false);
            break;
        case ARROWS_NUMBERS:
            oled_write_ln_P(PSTR("ARROWS"), false);
            break;
        case MOUSE:
            oled_write_ln_P(PSTR("MOUSE"), false);
            break;
        case NAVIGATION:
            oled_write_ln_P(PSTR("NAVIGATION"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    if (is_keyboard_master()) {
        oled_write_ln_P(PSTR("MASTER"), false);
    } else {
        oled_write_ln_P(PSTR("SLAVE"), false);
    }

    oled_write_ln_P(PSTR("          ^.___.^   "), false);
    oled_write_ln_P(PSTR(""), false);

    return false;
}


void pointing_device_init_user(void){
    set_auto_mouse_layer(MOUSE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t i_esc_timer;
    switch (keycode) {
    case I_ESC:
        uprintf("I_ESC\n");
        if (record->event.pressed) {
            i_esc_timer = timer_read();
        } else {
            if (timer_elapsed(i_esc_timer) < TAPPING_TERM) {
                tap_code(KC_I);
            } else {
                tap_code(KC_ESC);
            }
        }
        return false;
    case MACRO_CHANGE_LANG:
        uprintf("MACRO_CHANGE_LANG\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_SPC);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_SPC);
        }
        return false;
        break;
    case TMUX_1:
        uprintf("TMUX_1\n");
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_1);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_1);
        }
        return false;
        break;
    case TMUX_2:
        uprintf("TMUX_2\n");
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_2);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_2);
        }
        return false;
        break;
    case TMUX_3:
        uprintf("TMUX_3\n");
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_3);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_3);
        }
        return false;
        break;
    case TMUX_4:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_4);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_4);
        }
        return false;
        break;
    case TMUX_5:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_5);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_5);
        }
        return false;
        break;
    case TMUX_6:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_6);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_6);
        }
        return false;
        break;
    case TMUX_7:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_7);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_7);
        }
        return false;
        break;
    case TMUX_8:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_8);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_8);
        }
        return false;
        break;
    case TMUX_9:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_9);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_9);
        }
        return false;
        break;
    case TMUX_10:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_0);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_0);
        }
        return false;
        break;
    case NAV_TRM:
        uprintf("NAV_TRM\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_1);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_1);
        }
        return false;
        break;
    case NAV_BRO:
        uprintf("NAV_BRO\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_2);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_2);
        }
        return false;
        break;
    case NAV_TEL:
        uprintf("NAV_TEL\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_3);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_3);
        }
        return false;
        break;
    case NAV_XXX:
        uprintf("NAV_XXX\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_4);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_4);
        }
        return false;
        break;
    case TM_NWIN:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_A);
            unregister_code(KC_LCTL);
            unregister_code(KC_A);
            register_code(KC_C);
        } else {
            unregister_code(KC_C);
        }
        return false;
        break;
    case BRO_LEF:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);
            unregister_code(KC_TAB);
        }
        return false;
        break;
    case BRO_RIG:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_TAB);
        }
        return false;
        break;
    case BRO_ADL:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_D);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_D);
        }
        return false;
        break;
    case BRO_NEW:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_T);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_T);
        }
        return false;
        break;
    }
    return true;
}
