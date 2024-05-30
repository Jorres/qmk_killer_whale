#include QMK_KEYBOARD_H
#include "lib/add_keycodes.h"

enum layer_number {
    BASE = 0,
    MOUSE,
    NAVIGATION,
    SYMBOLS,
    NUMBERS,
};

enum custom_keycodes {
    LANG = SAFE_RANGE,
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
    TM_SESS,

    BRO_LEF,
    BRO_RIG,
    BRO_ADL,
    BRO_NEW,

    NAV_TRM,
    NAV_BRO,
    NAV_TEL,
    NAV_XXX,

    PRTSCR,

    I_ESC,
};

// キーマップの設定
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        // 左手
        KC_TAB,  KC_Q,      _______,          MO(NUMBERS), _______, _______,
        KC_LCTL, KC_A,      KC_W,             KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,             KC_D, KC_F, KC_G,
                 _______,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
                            MO(NAVIGATION),
        LT(SYMBOLS, KC_SPC), KC_ENT,
        _______, _______, R_CHMOD, _______,  _______, // <--- first 3 blanks are candidates for filling
        _______, _______,                    _______,


        // 右手
        PRTSCR,  _______, _______, KC_0,    LANG, KC_LBRC,
        KC_Y,    KC_U,    I_ESC,   KC_O,    KC_P, KC_QUOT,
        KC_H,    KC_J,    KC_K,    KC_L,    LT(MOUSE, KC_SCLN), KC_RSFT,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                            MO(NAVIGATION),
        KC_ENT,  LT(SYMBOLS, KC_BSPC),
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
    [MOUSE] = LAYOUT(
        // 左手
        _______, _______, _______, TO(BASE), _______,  _______,
        _______, _______, _______, _______,  _______,  _______,
        _______, KC_Z,    KC_S,    KC_MS_BTN2, KC_MS_BTN1, MOD_SCRL, // Z and S for Win combo
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
        _______, _______, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
        _______, TMUX_1,  TMUX_2,  TMUX_3,  TMUX_4,  TMUX_5,
        _______, _______, NAV_TRM, NAV_BRO, NAV_TEL, NAV_XXX,
                 _______, _______, _______, _______, TM_SESS,
                          KC_TRNS,
        BRO_LEF, BRO_NEW,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        _______, _______, _______, _______, _______, TO(BASE),
        TMUX_6,  TMUX_7,  TMUX_8,  TMUX_9,  TMUX_10, _______,
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
        _______, KC_ASTR, KC_EQUAL, KC_BSLS, KC_PIPE, _______,
        _______, KC_CIRC, KC_GRV,   KC_DLR,  KC_AT,    KC_EXLM,
                 _______, KC_MINS,  KC_LABK, KC_RABK, _______,
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
    ),
    [NUMBERS] = LAYOUT(
        // 左手
        _______, _______, _______,  _______, _______, _______,
        _______, _______, _______,  _______, _______, KC_LEFT,
        _______, _______, _______,  _______, _______, KC_UP,
                 _______, _______,  _______, _______, KC_DOWN,
                          _______,
        _______, _______,
        _______, _______, _______,  _______,          _______,
        _______, _______,                             _______,


        // 右手
        _______, _______, _______, _______, _______, TO(BASE),
        KC_RIGHT, KC_7,   KC_8,    KC_9,   _______, _______,
        KC_UP,    KC_4,   KC_5,    KC_6,   KC_0, _______,
        KC_DOWN,  KC_1,   KC_2,    KC_3,   KC_0,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    )
};

const uint16_t PROGMEM lgui[] = {KC_Z, KC_S, COMBO_END};
const uint16_t PROGMEM lkm[] = {KC_J, KC_L, COMBO_END};
combo_t key_combos[] = {
    COMBO(lkm, KC_MS_BTN1),
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
        case MOUSE:
            oled_write_ln_P(PSTR("MOUSE"), false);
            break;
        case SYMBOLS:
            oled_write_ln_P(PSTR("SYMBOLS"), false);
            break;
        case NUMBERS:
            oled_write_ln_P(PSTR("NUMBERS"), false);
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


static uint16_t i_esc_timer = 0;
static bool i_esc_pressed = false;

void matrix_scan_user(void) {
    if (i_esc_pressed && timer_elapsed(i_esc_timer) >= TAPPING_TERM) {
        i_esc_pressed = false;
        tap_code(KC_ESC);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case TM_SESS:
        uprintf("TM_SESS\n");
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_A);
            unregister_code(KC_LCTL);
            unregister_code(KC_A);
            register_code(KC_LSFT);
            register_code(KC_0);
        } else {
            unregister_code(KC_LSFT);
            unregister_code(KC_0);
        }
        return false;
    case PRTSCR:
        uprintf("PRTSCR\n");
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LSFT);
            register_code(KC_BSLS);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);
            unregister_code(KC_BSLS);
        }
        return false;
    case I_ESC:
        uprintf("I_ESC\n");
        if (record->event.pressed) {
            i_esc_timer = timer_read();
            i_esc_pressed = true;
        } else {
            if (timer_elapsed(i_esc_timer) < TAPPING_TERM) {
                tap_code(KC_I);
            }
            i_esc_pressed = false;
        }
        return false;
    case LANG:
        uprintf("LANG\n");
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
