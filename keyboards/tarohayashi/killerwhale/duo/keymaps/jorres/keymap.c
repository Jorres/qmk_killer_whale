#include QMK_KEYBOARD_H
#include "lib/add_keycodes.h"
#include "lib/common_killerwhale.h"

enum layer_number {
    BASE = 0,
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
    TMUX_COPY,
    TMUX_SEARCH,
    TMUX_HIGHLIGHT,

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

    DPAD_LEFT,
};

/* Luna settings */
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       40

#define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#define ANIM_SIZE           96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

uint32_t anim_timer = 0;

uint8_t current_frame = 0;

int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* Luna settings end */

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
        _______, _______, DPAD_LEFT, R_CHMOD,  _______, // <--- first 3 blanks are candidates for filling
        _______, _______,                    _______,


        // 右手
        PRTSCR,  KC_LGUI, KC_SPC,  KC_0,    KC_F11, KC_RSFT,
        KC_Y,    KC_U,    I_ESC,   KC_O,    KC_P, KC_LBRC,
        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                            MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______,
        _______, _______,                     _______
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
        _______, _______, _______, _______, _______, _______,
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

// const key_override_t backspace_key_override = ko_make_basic(MOD_MASK_CTRL, KC_H, KC_BSPC);

// const key_override_t **key_overrides = (const key_override_t *[]){
// 	&backspace_key_override,
// 	NULL
// };

const uint16_t PROGMEM lgui[] = {KC_Z, KC_S, COMBO_END};
const uint16_t PROGMEM lkm[] = {KC_J, KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM pkm[] = {KC_M, KC_COMM, KC_DOT, COMBO_END};

const uint16_t PROGMEM tmux_highlight[] = {KC_J, KC_P, COMBO_END};
const uint16_t PROGMEM tmux_search[] = {KC_M, KC_P, COMBO_END};
const uint16_t PROGMEM tmux_copy[] = {KC_U, KC_P, COMBO_END};

combo_t key_combos[] = {
    COMBO(lkm, KC_MS_BTN1),
    COMBO(pkm, KC_MS_BTN2),
    COMBO(lgui, KC_LGUI),

    COMBO(tmux_highlight, TMUX_HIGHLIGHT),
    COMBO(tmux_search, TMUX_SEARCH),
    COMBO(tmux_copy, TMUX_COPY),
};

void keyboard_post_init_user(void) {
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
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
    if (keycode != I_ESC && i_esc_pressed) {
        i_esc_pressed = false;
        tap_code(KC_I);
    }

    switch (keycode) {
    case DPAD_LEFT:
        uprintf("DPAD_LEFT\n");
        if (record->event.pressed) {
            // register_code(KC_LEFT);
            set_scroll_mode();
        } else {
            // unregister_code(KC_LEFT);
            unset_scroll_mode();
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
            register_code(KC_LCTL);
            register_code(KC_SLSH);
        } else {
            register_code(KC_LCTL);
            register_code(KC_SLSH);
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
    /* KEYBOARD PET STATUS START */
    case KC_LCTL:
    case KC_RCTL:
        if (record->event.pressed) {
            isSneaking = true;
        } else {
            isSneaking = false;
        }
        break;
    case KC_SPC:
        if (record->event.pressed) {
            isJumping  = true;
            showedJump = false;
        } else {
            isJumping = false;
        }
        break;

        /* KEYBOARD PET STATUS END */
    }
    return true;
}


/* Luna logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'sit2', 32x22px */
                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'walk2', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'run2', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   }};

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
                                                    {
                                                        0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'bark2', 32x22px */
                                                    {
                                                        0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                     },

                                                     /* 'sneak2', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     }};

    /* animation */
    void animate_luna(void) {
        /* jump */
        if (isJumping || !showedJump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y - 1);

            showedJump = true;
        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y);
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;

        /* current status */
        if (led_usb_state.caps_lock) {
            oled_write_raw_P(bark[current_frame], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[current_frame], ANIM_SIZE);

        } else if (current_wpm <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[current_frame], ANIM_SIZE);

        } else if (current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[current_frame], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[current_frame], ANIM_SIZE);
        }
    }

#    if OLED_TIMEOUT > 0
    /* the animation prevents the normal timeout from occuring */
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {

        oled_off();
        return;
    } else {
        oled_on();
    }
#    endif

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }
}

/* KEYBOARD PET END */


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_90;
    } else {
        return OLED_ROTATION_270;
    }
}

bool oled_task_user(void) {
    // oled_clear();

    /* KEYBOARD PET VARIABLES START */

    current_wpm   = get_current_wpm();
    led_usb_state = host_keyboard_led_state();

    /* KEYBOARD PET VARIABLES END */

    render_luna(0, 13);
    // oled_write_P(PSTR("Layer: "), false);

    // switch (get_highest_layer(layer_state)) {
    //     case BASE:
    //         oled_write_ln_P(PSTR("BASE"), false);
    //         break;
    //     case SYMBOLS:
    //         oled_write_ln_P(PSTR("SYMBOLS"), false);
    //         break;
    //     case NUMBERS:
    //         oled_write_ln_P(PSTR("NUMBERS"), false);
    //         break;
    //     case NAVIGATION:
    //         oled_write_ln_P(PSTR("NAVIGATION"), false);
    //         break;
    //     default:
    //         oled_write_ln_P(PSTR("Undefined"), false);
    // }

    // if (is_keyboard_master()) {
    //     oled_write_ln_P(PSTR("MASTER"), false);
    // } else {
    //     oled_write_ln_P(PSTR("SLAVE"), false);
    // }

    // oled_write_ln_P(PSTR("          ^.___.^   "), false);
    // oled_write_ln_P(PSTR(""), false);

    return false;
}
