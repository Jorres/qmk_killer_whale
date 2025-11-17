#include QMK_KEYBOARD_H

#include "lib/add_keycodes.h"
#include "lib/common_killerwhale.h"
#include "transactions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom_keycodes.h"
#include "process_record.h"
#include "rgb_layers.h"

#define LUNA_ENABLE  // Comment out to disable Luna

#ifdef LUNA_ENABLE
#include "luna.h"
#endif

enum layer_number {
    BASE = 0,
    NAVIGATION,
    SYMBOLS,
    NUMBERS,
    // QUD,
    // QUD_2,
    SC_MAIN,
    SC_SEC,
    SC_THI,
    HOGWARTS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        // 左手
         // KC_TAB,  KC_Q,      KC_1,    KC_2, KC_3, KC_4,
        KC_TAB,  KC_Q,      KC_1,    MO(NUMBERS), TO(HOGWARTS), KC_J,
        KC_LCTL, KC_A,      KC_W,       KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        MO(NAVIGATION),
        LT(SYMBOLS, KC_SPC), KC_ENT,
        _______, _______, KC_LALT, KC_ESC,  _______, // first four are d-pad on the left  half
        // KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,


        // 右手
        PRTSCR,  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE,
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  KC_SLSH,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
    [NAVIGATION] = LAYOUT(
        // 左手
        RGB_TOG, _______, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
        _______, TMUX_1,  TMUX_2,  TMUX_3,  TMUX_4,  TMUX_5,
        _______, _______, NAV_TRM, NAV_BRO, NAV_TEL, NAV_XX1,
                 _______, _______, _______, _______, TM_SESS,
                          KC_TRNS,
        BRO_LEF, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        _______, _______, _______, _______, _______, _______,
        TMUX_6,  TMUX_7,  TMUX_8,  TMUX_9,  TMUX_10, _______,
        NAV_XX2, NAV_XX3, KC_WBAK, KC_WREF, KC_WFWD, _______,
        _______, TM_NWIN, _______, _______, _______,
                                   KC_TRNS,
        BRO_ADL, BRO_RIG,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [SYMBOLS] = LAYOUT(
        // 左手
        UC(64), _______, _______,  _______, _______, _______,
        _______, KC_ASTR, KC_EQUAL, KC_BSLS, KC_PIPE, _______,
        _______, KC_CIRC, KC_GRV,   KC_DLR,  KC_AT,   KC_EXLM,
                 _______, KC_MINS,  KC_LABK, KC_RABK, _______,
                          _______,
        _______, _______,
        _______, _______, _______,  _______,          _______,
        _______, _______,                             _______,


        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, KC_LCBR, KC_RCBR, KC_PLUS, _______, _______,
        KC_TILD, KC_LPRN, KC_RPRN, KC_HASH, KC_AMPR, _______,
        KC_PERC, KC_LBRC, KC_RBRC, KC_UNDS, _______,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [NUMBERS] = LAYOUT(
        // 左手
        _______, _______, LCTL(KC_LEFT), _______, LCTL(KC_RIGHT), _______,
        _______, _______, RCS(KC_LEFT),  _______, RCS(KC_RIGHT),  KC_LEFT,
        _______, _______, _______,       _______, _______,        KC_UP,
                 _______, _______,       _______, _______,        KC_DOWN,
                          _______,
        _______, _______,
        _______, _______, _______,  _______,          _______,
        _______, _______,                             _______,


        // 右手
        _______, _______, _______, _______, _______, _______,
        KC_RIGHT, KC_7,   KC_8,    KC_9,    _______, _______,
        KC_UP,    KC_4,   KC_5,    KC_6,    _______, _______,
        KC_DOWN,  KC_1,   KC_2,    KC_3,    KC_0,
                                   _______,
        _______, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    // [QUD] = LAYOUT(
    //     // 左手
    //     KC_TAB,  KC_Q,      KC_1,    KC_2, KC_3, KC_4,
    //     KC_LCTL, KC_LEFT,      KC_UP,       KC_E, KC_R, KC_T,
    //     KC_LSFT, KC_Z,      KC_DOWN,       KC_RIGHT, KC_F, KC_G,
    //     _______,   KC_X, KC_C, KC_V, KC_B,
    //     MO(NAVIGATION),
    //     LT(SYMBOLS, KC_SPC), KC_ENT,
    //     KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
    //     _______, _______,                    _______,


    //     // 右手
    //     KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
    //     KC_Y,    KC_U,    I_ESC,    KC_O,    KC_P,    OSL(QUD_2),
    //     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, QUOT_RSFT,
    //     KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
    //                                         KC_LEFT_ALT,
    //     KC_BSPC, KC_Q,
    //     KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
    //     _______, _______,                     _______
    // ),
    // [QUD_2] = LAYOUT(
    //     // 左手
    //     KC_TAB,  KC_Q,      KC_END,    MO(NUMBERS), _______, TO(BASE),
    //     KC_LCTL, KC_A,      KC_W,       KC_E, KC_R, KC_T,
    //     KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
    //     _______,   KC_X, KC_C, KC_V, KC_B,
    //     MO(NAVIGATION),
    //     LT(SYMBOLS, KC_SPC), KC_ENT,
    //     _______, _______, _______, R_CHMOD,  _______,
    //     _______, _______,                    _______,


    //     // 右手
    //     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,
    //     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
    //     KC_F11,  KC_F12,  KC_A,  KC_W,  KC_E,  _______,
    //     KC_R,  KC_T,  KC_Z,  KC_S,  KC_D,
    //                                         KC_0,
    //     KC_F,    KC_G,
    //     KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
    //     _______, _______,                    _______
    // ),
    [SC_MAIN] = LAYOUT(
        // 左手
        CTLSHFT,  KC_Q,      KC_1,       KC_2, KC_3, KC_4,
        LM(SC_SEC, MOD_LCTL), KC_A,      KC_W,       KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        KC_ESC,
        LM(SC_THI, MOD_LSFT), KC_ENT,
        // _______, _______, KC_LALT, KC_ESC,  _______, // first four are d-pad on the left  half
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,


        // 右手
        TO(BASE),  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE,
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  KC_SLSH,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
    [SC_SEC] = LAYOUT(
        // 左手
        CTLSHFT,  KC_Q,      KC_1,       KC_2, KC_3, KC_4,
        _______, KC_A,      KC_5,       KC_6, KC_7, KC_8,
        _______, KC_Z,      KC_F1,       KC_F2, KC_F3, KC_F4,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        KC_ESC,
        KC_ESC, KC_ENT,
        // _______, _______, KC_LALT, KC_ESC,  _______, // first four are d-pad on the left  half
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,


        // 右手
        TO(BASE),  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE,
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  KC_SLSH,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
    [SC_THI] = LAYOUT(
        // 左手
        CTLSHFT,  KC_Q,      KC_1,       KC_2, KC_3, KC_4,
        _______, KC_A,      KC_5,       KC_6, KC_7, KC_8,
        _______, KC_Z,      KC_F1,       KC_F2, KC_F3, KC_F4,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        KC_ESC,
        KC_ESC, KC_ENT,
        // _______, _______, KC_LALT, KC_ESC,  _______, // first four are d-pad on the left  half
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,


        // 右手
        TO(BASE),  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE,
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  KC_SLSH,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
    [HOGWARTS] = LAYOUT(
        // 左手
         KC_TAB,  KC_Q,      KC_1,    KC_2, KC_3, KC_4, // this line modified from base
        KC_LCTL, KC_A,      KC_W,       KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
        KC_X,   KC_X, KC_C, KC_V, KC_B,
        MO(NAVIGATION),
        KC_SPC, KC_ENT, // first key modified
        _______, _______, KC_LALT, KC_ESC,  _______,
        // KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,


        // 右手
        TO(BASE),  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE, // first key modified
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  KC_SLSH,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
    ),
};

const uint16_t PROGMEM lgui[] = {KC_Z, KC_S, COMBO_END};
const uint16_t PROGMEM lalt[] = {KC_X, KC_D, COMBO_END};
const uint16_t PROGMEM lkm[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM pkm[] = {KC_M, KC_COMM, KC_DOT, COMBO_END};

const uint16_t PROGMEM tmux_copy[] = {KC_U, KC_DELETE, COMBO_END};
const uint16_t PROGMEM tmux_highlight[] = {KC_J, KC_DELETE, COMBO_END};
const uint16_t PROGMEM tmux_search[] = {KC_M, KC_DELETE, COMBO_END};

const uint16_t PROGMEM bootloader[] = {KC_Q, KC_W, KC_A, KC_S,  COMBO_END};

combo_t key_combos[] = {
    COMBO(lkm, KC_MS_BTN1),
    COMBO(pkm, KC_MS_BTN2),
    COMBO(lgui, KC_LGUI),
    COMBO(lalt, KC_LALT),

    COMBO(tmux_copy, TMUX_COPY),
    COMBO(tmux_highlight, TMUX_HIGHLIGHT),
    COMBO(tmux_search, TMUX_SEARCH),

    COMBO(bootloader, QK_BOOTLOADER),
};

void matrix_scan_user(void) {
    custom_keycodes_matrix_scan();
    rgb_layers_scan(BASE);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_custom_keycodes(keycode, record)) {
        return false;
    }

#ifdef LUNA_ENABLE
    luna_process_record(keycode, record);
#endif

    return true;
}


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_90;
    } else {
        return rotation;
    }
}

void write_layer_to_oled(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case BASE:
            oled_write_ln_P(PSTR("BASE"), false);
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
        // case QUD:
        //     oled_write_ln_P(PSTR("QUD"), false);
        //     break;
        // case QUD_2:
        //     oled_write_ln_P(PSTR("QUD_2"), false);
        //     break;
        case SC_MAIN:
            oled_write_ln_P(PSTR("SC_MAIN"), false);
            break;
        case SC_SEC:
            oled_write_ln_P(PSTR("SC_SEC"), false);
            break;
        case SC_THI:
            oled_write_ln_P(PSTR("SC_THI"), false);
            break;
        case HOGWARTS:
            oled_write_ln_P(PSTR("HOGWARTS"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    if (is_keyboard_left()) {
        oled_write_ln_P(PSTR("left"), false);
    } else {
        oled_write_ln_P(PSTR("right"), false);
    }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
#ifdef LUNA_ENABLE
        current_wpm   = get_current_wpm();
        led_usb_state = host_keyboard_led_state();
        render_luna(0, 13);
#else
        oled_clear();
        write_layer_to_oled();
#endif
    } else {
        oled_clear();
        write_layer_to_oled();
    }

    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return TAPPING_TERM;
    }
}

void matrix_init_user(void) {
    rgb_layers_init();
}

void housekeeping_task_user(void) {
    rgb_layers_task(BASE);

    static uint32_t last_sync = 0;
    if (timer_elapsed32(last_sync) > 500) {
        animate_base_layer();
        last_sync = timer_read32();
    }
}
