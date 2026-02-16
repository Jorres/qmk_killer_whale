#include QMK_KEYBOARD_H

enum layer_number {
    BASE = 0,
    NAVIGATION,
    SYMBOLS,
    NUMBERS,
};

enum custom_keycodes {
    PRTSCR = SAFE_RANGE,
    I_ESC,
    LBRC_RCTL,
    QUOT_RSFT,
    SLSH_SLOW,
    LANG,
    HARPOON_1,
    HARPOON_2,
    TMUX_1, TMUX_2, TMUX_3, TMUX_4, TMUX_5,
    TMUX_6, TMUX_7, TMUX_8, TMUX_9, TMUX_10,
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
    TG_SRCH,
};

#define QUOT_RSFT_TAPPING_TERM 50

const uint16_t PROGMEM lgui_combo[] = {KC_Z, KC_S, COMBO_END};
const uint16_t PROGMEM lalt_combo[] = {KC_X, KC_D, COMBO_END};
const uint16_t PROGMEM lkm_combo[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM pkm_combo[] = {KC_M, KC_COMM, KC_DOT, COMBO_END};

const uint16_t PROGMEM tmux_copy_combo[] = {KC_U, HARPOON_2, COMBO_END};
const uint16_t PROGMEM tmux_highlight_combo[] = {KC_J, HARPOON_2, COMBO_END};
const uint16_t PROGMEM tmux_search_combo[] = {KC_M, HARPOON_2, COMBO_END};

const uint16_t PROGMEM bootloader_combo[] = {KC_Q, KC_W, KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM bootloader_right_combo[] = {KC_O, KC_L, KC_P, KC_F11, COMBO_END};

combo_t key_combos[] = {
    COMBO(lkm_combo, KC_MS_BTN1),
    COMBO(pkm_combo, KC_MS_BTN2),
    COMBO(lgui_combo, KC_LGUI),
    COMBO(lalt_combo, KC_LALT),
    COMBO(tmux_copy_combo, TMUX_COPY),
    COMBO(tmux_highlight_combo, TMUX_HIGHLIGHT),
    COMBO(tmux_search_combo, TMUX_SEARCH),
    COMBO(bootloader_combo, QK_BOOTLOADER),
    COMBO(bootloader_right_combo, QK_BOOTLOADER),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
    KC_TAB,   KC_Q,      KC_1,      MO(NUMBERS),     _______,  _______,               PRTSCR,    HARPOON_1,   HARPOON_2, KC_0,     KC_F11,    _______,
    KC_LCTL,  KC_A,      KC_W,      KC_E,            KC_R,     KC_T,                  KC_Y,      KC_U,        I_ESC,     KC_O,     KC_P,      LBRC_RCTL,
    KC_LSFT,  KC_Z,      KC_S,      KC_D,            KC_F,     KC_G,                  KC_H,      KC_J,        KC_K,      KC_L,     KC_SCLN,   QUOT_RSFT,
    _______,  _______,   KC_X,      KC_C,            KC_V,     KC_B,                  KC_N,      KC_M,        KC_COMM,  KC_DOT,    SLSH_SLOW, _______,
                                    MO(NAVIGATION),  MO(SYMBOLS),                                MO(SYMBOLS), MO(NAVIGATION),
                                    KC_SPC,          _______,                                    KC_BSPC,     _______,
                                    KC_ENT,          _______,                                    _______,     LANG,
                                    _______,         _______,                                    _______,     _______
  ),

  [NAVIGATION] = LAYOUT(
    _______,  _______,   KC_VOLD,   KC_VOLU,   KC_BRID,  KC_BRIU,                  _______,   _______,   _______,   _______,  _______,   _______,
    _______,  TMUX_1,    TMUX_2,    TMUX_3,    TMUX_4,   TMUX_5,                   TMUX_6,    TMUX_7,    TMUX_8,    TMUX_9,   TMUX_10,   _______,
    _______,  _______,   NAV_TRM,   NAV_BRO,   NAV_TEL,  NAV_XX1,                  NAV_XX2,   NAV_XX3,   KC_WBAK,   KC_WREF,  KC_WFWD,   _______,
    _______,  _______,   _______,   TG_SRCH,   _______,  TM_SESS,                  _______,   _______,   TM_NWIN,   _______,  _______,   _______,
                                    _______,   _______,                                       _______,   _______,
                                    BRO_LEF,   _______,                                       BRO_ADL,   BRO_RIG,
                                    _______,   _______,                                       _______,   _______,
                                    _______,   _______,                                       _______,   _______
  ),

  [SYMBOLS] = LAYOUT(
    _______,  _______,   _______,   _______,   _______,  _______,                  _______,   _______,   _______,   _______,  _______,   _______,
    _______,  KC_ASTR,   KC_EQUAL,  UC(0x005C), UC(0x007C), UC(0x0026),           UC(0x003F), UC(0x007B), UC(0x007D), KC_PLUS,  UC(0x003A), _______,
    _______,  UC(0x005E), UC(0x0060), UC(0x0024), UC(0x0040), KC_EXLM,            UC(0x007E), KC_LPRN,   KC_RPRN,   UC(0x0023), UC(0x003B), UC(0x0027),
    _______,  _______,   KC_MINS,   UC(0x003C), UC(0x003E), UC(0x0022),           KC_PERC,   UC(0x005B), UC(0x005D), KC_UNDS,   UC(0x002F), _______,
                                    _______,   _______,                                       _______,   _______,
                                    _______,   _______,                                       _______,   _______,
                                    _______,   _______,                                       _______,   _______,
                                    _______,   _______,                                       _______,   _______
  ),

  [NUMBERS] = LAYOUT(
    _______,  _______,   LCTL(KC_LEFT), _______,   LCTL(KC_RIGHT), _______,        _______,  _______,   _______,   _______,  _______,   _______,
    _______,  _______,   RCS(KC_LEFT),  _______,   RCS(KC_RIGHT),  KC_LEFT,        KC_RIGHT, KC_7,      KC_8,      KC_9,     _______,   _______,
    _______,  _______,   _______,       _______,   _______,        KC_UP,          KC_UP,    KC_4,      KC_5,      KC_6,     _______,   _______,
    _______,  _______,   _______,       _______,   _______,        KC_DOWN,        KC_DOWN,  KC_1,      KC_2,     KC_3,      KC_0,      _______,
                                        _______,   _______,                                  _______,   _______,
                                        _______,   _______,                                  _______,   _______,
                                        _______,   _______,                                  _______,   _______,
                                        _______,   _______,                                  _______,   _______
  ),
};

// --- Custom keycode implementation ---

static uint16_t i_esc_timer = 0;
static bool i_esc_pressed = false;

static uint16_t r_sft_timer = 0;
static bool r_sft_alone = false;
static bool registered_r_sft = false;

static uint16_t r_ctl_timer = 0;
static bool r_ctl_alone = false;
static bool registered_r_ctl = false;

static uint16_t slsh_timer = 0;
static bool slsh_alone = false;
static bool registered_slsh = false;

void matrix_scan_user(void) {
    if (i_esc_pressed && timer_elapsed(i_esc_timer) >= TAPPING_TERM) {
        i_esc_pressed = false;
        tap_code(KC_ESC);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle key interruptions for tap-hold keys
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

    if (keycode != SLSH_SLOW && slsh_alone) {
        slsh_alone = false;
        if (timer_elapsed(slsh_timer) < TAPPING_TERM) {
            tap_code(KC_SLSH);
        }
        registered_slsh = false;
    }

    switch (keycode) {
    case LBRC_RCTL:
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
        if (record->event.pressed) {
            r_sft_timer = timer_read();
            r_sft_alone = true;
        } else {
            if (registered_r_sft) {
                unregister_code(KC_RSFT);
            } else if (r_sft_alone && timer_elapsed(r_sft_timer) < TAPPING_TERM) {
                tap_code(KC_QUOT);
            }
            registered_r_sft = false;
            r_sft_timer = 0;
            r_sft_alone = false;
        }
        return false;
    case SLSH_SLOW:
        if (record->event.pressed) {
            slsh_timer = timer_read();
            slsh_alone = true;
        } else {
            if (!registered_slsh && slsh_alone && timer_elapsed(slsh_timer) < TAPPING_TERM) {
                tap_code(KC_SLSH);
            }
            registered_slsh = false;
            slsh_timer = 0;
            slsh_alone = false;
        }
        return false;
    case PRTSCR:
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
        if (record->event.pressed) {
            register_code(KC_COMMA);
            register_code(KC_1);
        } else {
            unregister_code(KC_COMMA);
            unregister_code(KC_1);
        }
        return false;
    case HARPOON_2:
        if (record->event.pressed) {
            register_code(KC_COMMA);
            register_code(KC_2);
        } else {
            unregister_code(KC_COMMA);
            unregister_code(KC_2);
        }
        return false;
    case LANG:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_SPC);
            unregister_code(KC_LGUI);
            unregister_code(KC_SPC);
        }
        return false;
    case TMUX_1: case TMUX_2: case TMUX_3: case TMUX_4: case TMUX_5:
    case TMUX_6: case TMUX_7: case TMUX_8: case TMUX_9: case TMUX_10:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_1 + (keycode - TMUX_1));
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_1 + (keycode - TMUX_1));
        }
        return false;
    case TMUX_COPY:
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
    case NAV_TRM: case NAV_BRO: case NAV_TEL:
    case NAV_XX1: case NAV_XX2: case NAV_XX3:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_1 + (keycode - NAV_TRM));
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_1 + (keycode - NAV_TRM));
        }
        return false;
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
    case BRO_RIG:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_TAB);
        }
        return false;
    case BRO_ADL:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_D);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_D);
        }
        return false;
    case TG_SRCH:
        if (record->event.pressed) {
            tap_code(KC_ESC);
            tap_code(KC_ESC);
            register_code(KC_LCTL);
            tap_code(KC_F);
            unregister_code(KC_LCTL);
        }
        return false;
    }

    return true;
}
