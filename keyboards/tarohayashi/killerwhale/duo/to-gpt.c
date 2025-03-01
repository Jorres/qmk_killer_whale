#include QMK_KEYBOARD_H

#include "lib/add_keycodes.h"
#include "lib/common_killerwhale.h"
#include "transactions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum layer_number {
    BASE = 0,
    NAVIGATION,
    SYMBOLS,
    NUMBERS,
    QUD,
    QUD_2,
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
    TMUX_COPY,
    TMUX_SEARCH,
    TMUX_HIGHLIGHT,

    BRO_LEF,
    BRO_RIG,
    BRO_ADL,

    NAV_TRM,
    NAV_BRO,
    NAV_TEL,

    NAV_XX1,
    NAV_XX2,
    NAV_XX3,

    PRTSCR,

    I_ESC,

    KB_BRID,
    KB_BRIU,

    LBRC_RCTL,
    QUOT_RSFT,

    HARPOON_1,
    HARPOON_2,
};

/* Luna settings */
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       40

#define ANIM_FRAME_DURATION 180  // how long each frame lasts in ms
#define ANIM_SIZE           96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

uint32_t anim_timer = 0;

uint8_t current_frame = 0;

int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* Luna settings end */


uint32_t slave_right_half_timer = 0;
bool is_slavemode_right_checked = false;

// キーマップの設定
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        // 左手
         // KC_TAB,  KC_Q,      KC_1,    KC_2, KC_3, KC_4,
        KC_TAB,  KC_Q,      RGB_TOG,    MO(NUMBERS), _______, _______,
        KC_LCTL, KC_A,      KC_W,       KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        MO(NAVIGATION),
        LT(SYMBOLS, KC_SPC), KC_ENT,
        KC_F1, KC_F2, KC_F3, KC_F4,  _______, // first four are d-pad on the left  half
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
        _______, _______, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
        _______, TMUX_1,  TMUX_2,  TMUX_3,  TMUX_4,  TMUX_5,
        _______, _______, NAV_TRM, NAV_BRO, NAV_TEL, NAV_XX1,
                 _______, _______, _______, _______, TM_SESS,
                          KC_TRNS,
        BRO_LEF, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        KB_BRID, KB_BRIU, _______, _______, _______, _______,
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

static uint16_t i_esc_timer = 0;
static bool i_esc_pressed = false;

static uint16_t k_scroll_timer = 0;
static bool k_scroll_pressed = false;

static uint16_t r_sft_timer = 0;
static bool r_sft_alone = false;
static bool registered_r_sft = false;

static uint16_t r_ctl_timer = 0;
static bool r_ctl_alone = false;
static bool registered_r_ctl = false;

// behavior of my custom right modifiers:
// if tapped alone within tapping term, should just send their key.
// if tapped, held and tapped other key while holding (happens when fast typing),
//   MUST also register the regular key and do earlier release
// if tapped, held for TAPPING_TERM,
//
// we have to write code in multiple places: variable declarations,
// code in process_record_user that should trigger when the SUBSEQUENT key is pressed,
// and code in process_record_user's switch to trigger when the actual key is pressed.

void matrix_scan_user(void) {
    if (is_keyboard_master()) {
        is_slavemode_right_checked = false;
    } else if (!is_keyboard_left()) {
        if (!is_slavemode_right_checked) {
            slave_right_half_timer = timer_read();
            is_slavemode_right_checked = true;
        } else if (timer_elapsed(slave_right_half_timer) > 10000) {
            reset_keyboard();
        }
    } else {
        is_slavemode_right_checked = false;
    }

    if (i_esc_pressed && timer_elapsed(i_esc_timer) >= TAPPING_TERM) {
        i_esc_pressed = false;
        tap_code(KC_ESC);
    }

    if (k_scroll_pressed && timer_elapsed(k_scroll_timer) >= TAPPING_TERM) {
        k_scroll_pressed = false;
        set_scroll_mode();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode != I_ESC && i_esc_pressed) {
        i_esc_pressed = false;
        tap_code(KC_I);
    }

    if (keycode != LBRC_RCTL && r_ctl_alone) {
        r_ctl_alone = false;
        if (timer_elapsed(r_ctl_timer) < TAPPING_TERM) {
            tap_code(KC_LBRC);
        } else if (!registered_r_ctl) {
            registered_r_ctl = true;
            register_code(KC_RCTL);
        }
    }

    if (keycode != QUOT_RSFT && r_sft_alone) {
        r_sft_alone = false;
        if (timer_elapsed(r_sft_timer) < QUOT_RSFT_TAPPING_TERM) {
            tap_code(KC_QUOT);
        } else if (!registered_r_sft) {
            registered_r_sft = true;
            register_code(KC_RSFT);
        }
    }

    switch (keycode) {
    case LBRC_RCTL:
        uprintf("LBRC_RCTL\n");
        if (record->event.pressed) {
            r_ctl_timer = timer_read();
            r_ctl_alone = true;
        } else {
            if (registered_r_ctl) {
                unregister_code(KC_RCTL);
            } else if (r_ctl_alone && timer_elapsed(r_ctl_timer) < TAPPING_TERM) {
                tap_code(KC_LBRC);
            }
            registered_r_ctl = false;
            r_ctl_timer = 0;
            r_ctl_alone = false;
        }
        return false;
    case QUOT_RSFT:
        uprintf("QUOT_RSFT\n");
        if (record->event.pressed) {
            set_scroll_mode(); // yes, this key also doubles as trackball scroll enabler
            r_sft_timer = timer_read();
            r_sft_alone = true;
        } else {
            if (registered_r_sft) {
                unregister_code(KC_RSFT);
            } else if (r_sft_alone && timer_elapsed(r_sft_timer) < TAPPING_TERM) {
                tap_code(KC_QUOT);
            }
            unset_scroll_mode();
            registered_r_sft = false;
            r_sft_timer = 0;
            r_sft_alone = false;
        }
        return false;
    case KB_BRIU:
        uprintf("KB_BRIU\n");
        if (record->event.pressed) {
            // rgblight_increase_val();
        }
        return false;
    case KB_BRID:
        uprintf("KB_BRID\n");
        if (record->event.pressed) {
            // rgblight_decrease_val();
        }
        return false;
    case TMUX_COPY:
        uprintf("TMUX_COPY\n");
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_A);
            unregister_code(KC_LCTL);
            unregister_code(KC_A);
            register_code(KC_LSFT);
            register_code(KC_S);
        } else {
            unregister_code(KC_LSFT);
            unregister_code(KC_S);
        }
        return false;
    case TMUX_SEARCH:
        uprintf("TMUX_SEARCH\n");
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_A);
            unregister_code(KC_LCTL);
            unregister_code(KC_A);
            tap_code(KC_LBRC);
            register_code(KC_LSFT);
            register_code(KC_SLSH);
        } else {
            unregister_code(KC_LSFT);
            unregister_code(KC_SLSH);
        }
        return false;
    case TMUX_HIGHLIGHT:
        uprintf("TMUX_HIGHLIGHT\n");
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_A);
            unregister_code(KC_LCTL);
            unregister_code(KC_A);
            register_code(KC_LBRC);
        } else {
            unregister_code(KC_LBRC);
        }
        return false;
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
            if (i_esc_pressed && timer_elapsed(i_esc_timer) < TAPPING_TERM) {
                tap_code(KC_I);
            }
            i_esc_pressed = false;
        }
        return false;
    case HARPOON_1:
        uprintf("HARPOON_1\n");
        if (record->event.pressed) {
            register_code(KC_COMMA);
            register_code(KC_1);
        } else {
            unregister_code(KC_COMMA);
            unregister_code(KC_1);
        }
        return false;
    case HARPOON_2:
        uprintf("HARPOON_2\n");
        if (record->event.pressed) {
            register_code(KC_COMMA);
            register_code(KC_2);
        } else {
            unregister_code(KC_COMMA);
            unregister_code(KC_2);
        }
        return false;
    case LANG:
        uprintf("LANG\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_SPC);
            unregister_code(KC_LGUI);
            unregister_code(KC_SPC);
        } else {
            // avoid long-pressing to accidentally trigger the third language
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
    case NAV_XX1:
        uprintf("NAV_XX1\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_4);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_4);
        }
        return false;
        break;
    case NAV_XX2:
        uprintf("NAV_XX2\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_5);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_5);
        }
        return false;
        break;
    case NAV_XX3:
        uprintf("NAV_XX3\n");
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_6);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_6);
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
    }
    return true;
}


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_90;
    } else {
        return rotation;
    }
}

// void suspend_power_down_user(void) {
//     uprintf("power down\n");
//     add_debug = "suspended";
// }

// void suspend_wakeup_init_user(void) {
//     uprintf("wakeup init\n");
//     add_debug = "woke up";
// }

uint32_t oled_countdown_timer = 0;
int oled_countdown_value = 10;

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
        case QUD:
            oled_write_ln_P(PSTR("QUD"), false);
            break;
        case QUD_2:
            oled_write_ln_P(PSTR("QUD_2"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // if (is_keyboard_master()) {
    //     oled_write_ln_P(PSTR("MASTER"), false);
    // } else {
    //     char a = '0';
    //     a += debug_indicator;
    //     char* res = (char*)malloc(20);
    //     sprintf(res, "%s%c", "Slave debug", a);
    //     oled_write_ln_P(PSTR(res), false);
    //     // if (debug_indicator) {
    //     //     oled_write_ln_P(PSTR("SLAVE debug 1"), false);
    //     // } else {
    //     //     oled_write_ln_P(PSTR("SLAVE debug 2"), false);
    //     // }
    // }

    if (is_keyboard_left()) {
        oled_write_ln_P(PSTR("left"), false);
    } else {
        oled_write_ln_P(PSTR("right"), false);
    }

    // Countdown logic
    if (timer_elapsed(oled_countdown_timer) > 1000) {  // Check if 1 second has passed
        oled_countdown_timer = timer_read();  // Reset the timer
        oled_write_ln_P(PSTR("\n"), false);  // New line for the number

        char number_str[3];
        snprintf(number_str, sizeof(number_str), "%d", oled_countdown_value);
        oled_write_ln(number_str, false);  // Print the current countdown value

        if (oled_countdown_value > 1) {
            oled_countdown_value--;
        } else {
            oled_countdown_value = 10;  // Reset to 10
        }
    }
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        current_wpm   = get_current_wpm();
        led_usb_state = host_keyboard_led_state();
        render_luna(0, 13);
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
