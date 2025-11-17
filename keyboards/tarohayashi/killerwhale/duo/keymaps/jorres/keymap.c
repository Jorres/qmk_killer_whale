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
    // QUD,
    // QUD_2,
    SC_MAIN,
    SC_SEC,
    SC_THI,
    HOGWARTS,
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

    CTLSHFT,
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

// キーマップの設定
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

// Global flag to indicate BASE layer animation is active
bool rgb_animation_active = false;

// Base layer animation state
uint16_t base_anim_timer = 0;  // Non-static for external access
#define BASE_ANIM_INTERVAL 500  // Change color every 500ms

// Corner LED indices - global addressing (left: 0-32, right: 33-65)
static const uint8_t corner_leds_left[] = {0, 1, 2, 27};  // Left half corners
static const uint8_t corner_leds_right[] = {33, 34, 35, 60}; // Right half corners
#define CORNER_LED_COUNT_PER_HALF 4

// Track if RGB needs update (for slave sync)
static uint8_t last_synced_layer = 255;

// Color palette for base layer animation
static const uint8_t base_anim_colors[][3] = {
    {HSV_WHITE},
    {HSV_CYAN},
    {HSV_ORANGE}
};
#define BASE_ANIM_COLOR_COUNT 3

// Current color index for each corner LED (track state to reapply all each frame)
static uint8_t corner_led_colors[CORNER_LED_COUNT_PER_HALF] = {0, 0, 0, 0};  // All start white (index 0)

// Layer color definitions (HSV for corner LEDs)
// Format: {H, S, V} - same corners for all layers, just different colors
static const uint8_t layer_corner_colors[][3] = {
    {HSV_WHITE},   // BASE (0)
    {HSV_YELLOW},  // NAVIGATION (1)
    {HSV_WHITE},   // SYMBOLS (2)
    {HSV_AZURE},   // NUMBERS (3)
    {HSV_WHITE},   // SC_MAIN (4)
    {HSV_AZURE},   // SC_SEC (5)
    {HSV_WHITE},   // SC_THI (6)
    {HSV_CORAL},   // HOGWARTS (7)
};

// Track current layer for change detection
static uint8_t current_rgb_layer = 0;

// Helper function to apply all LED colors for BOTH halves
void apply_base_layer_leds(void) {
    // Apply to left half corners
    for (int i = 0; i < CORNER_LED_COUNT_PER_HALF; i++) {
        uint8_t color_idx = corner_led_colors[i];
        uint8_t h = base_anim_colors[color_idx][0];
        uint8_t s = base_anim_colors[color_idx][1];
        uint8_t v = base_anim_colors[color_idx][2];
        rgblight_sethsv_at(h, s, v, corner_leds_left[i]);
    }
    // Apply same colors to right half corners (mirror effect)
    for (int i = 0; i < CORNER_LED_COUNT_PER_HALF; i++) {
        uint8_t color_idx = corner_led_colors[i];
        uint8_t h = base_anim_colors[color_idx][0];
        uint8_t s = base_anim_colors[color_idx][1];
        uint8_t v = base_anim_colors[color_idx][2];
        rgblight_sethsv_at(h, s, v, corner_leds_right[i]);
    }
}

// Helper function to initialize base layer animation (all corners to white)
void init_base_layer_animation(void) {
    // Clear all LEDs first (both halves)
    rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);
    // Reset all corner LEDs to white (color index 0)
    for (int i = 0; i < CORNER_LED_COUNT_PER_HALF; i++) {
        corner_led_colors[i] = 0;  // White
    }
    apply_base_layer_leds();
}

// Helper function to change one random corner LED to a random color
void animate_base_layer(void) {
    // Pick a random corner LED index
    uint8_t corner_idx = rand() % CORNER_LED_COUNT_PER_HALF;
    // Pick a random color from palette
    uint8_t color_idx = rand() % BASE_ANIM_COLOR_COUNT;
    // Update the tracked color
    corner_led_colors[corner_idx] = color_idx;
    // Update both halves (same corner on both sides)
    uint8_t h = base_anim_colors[color_idx][0];
    uint8_t s = base_anim_colors[color_idx][1];
    uint8_t v = base_anim_colors[color_idx][2];
    rgblight_sethsv_at(h, s, v, corner_leds_left[corner_idx]);
    rgblight_sethsv_at(h, s, v, corner_leds_right[corner_idx]);
}

// Helper function to apply static layer colors (non-BASE layers)
void apply_static_layer_colors(uint8_t layer_idx) {
    // Clamp layer index to valid range
    if (layer_idx > 7) layer_idx = 0;

    // Set all LEDs to black first (both halves)
    rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);

    // Set corner LEDs to layer color
    uint8_t h = layer_corner_colors[layer_idx][0];
    uint8_t s = layer_corner_colors[layer_idx][1];
    uint8_t v = layer_corner_colors[layer_idx][2];

    // Set both halves
    for (int i = 0; i < CORNER_LED_COUNT_PER_HALF; i++) {
        rgblight_sethsv_at(h, s, v, corner_leds_left[i]);
        rgblight_sethsv_at(h, s, v, corner_leds_right[i]);
    }
}

// Main function called on layer change - applies appropriate lighting
void apply_layer_lighting(layer_state_t state) {
    uint8_t new_layer = get_highest_layer(state);

    if (new_layer == BASE) {
        // Entering BASE layer: start animation
        if (!rgb_animation_active) {
            rgb_animation_active = true;
            srand(timer_read());
            // Clear all LEDs first, then apply animation colors (both halves)
            rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);
            apply_base_layer_leds();
            base_anim_timer = timer_read();
        }
    } else {
        // Not BASE layer: apply static colors
        if (rgb_animation_active) {
            rgb_animation_active = false;
        }
        apply_static_layer_colors(new_layer);
    }

    current_rgb_layer = new_layer;
    last_synced_layer = new_layer;
}

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
    if (i_esc_pressed && timer_elapsed(i_esc_timer) >= TAPPING_TERM) {
        i_esc_pressed = false;
        tap_code(KC_ESC);
    }

    if (k_scroll_pressed && timer_elapsed(k_scroll_timer) >= TAPPING_TERM) {
        k_scroll_pressed = false;
        set_scroll_mode();
    }

    if (rgb_animation_active && get_highest_layer(layer_state) == BASE) {
        if (timer_elapsed(base_anim_timer) > BASE_ANIM_INTERVAL) {
            animate_base_layer();
            base_anim_timer = timer_read();
        }
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
    case CTLSHFT:
        if (record->event.pressed) {
            register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
        } else {
            unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT));
        }
        return false;
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

void matrix_init_user(void) {
    init_base_layer_animation();
    rgb_animation_active = true;
    srand(timer_read());
    base_anim_timer = timer_read();
}

// This runs on BOTH master and slave - catches layer changes on the slave
// since layer_state_set_user() is NOT called on slave when state is synced
void housekeeping_task_user(void) {
    uint8_t current_layer = get_highest_layer(layer_state);

    // Check if layer changed (works for both master and slave)
    if (current_layer != last_synced_layer) {
        apply_layer_lighting(layer_state);
    }

    static uint32_t last_sync = 0;
    if (timer_elapsed32(last_sync) > 500) {
        animate_base_layer();
        last_sync = timer_read32();
    }
}
