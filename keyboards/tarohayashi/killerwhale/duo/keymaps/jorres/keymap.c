#include QMK_KEYBOARD_H

#include "lib/add_keycodes.h"
#include "lib/common_killerwhale.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom_keycodes.h"
#include "process_record.h"
#include "rgb_layers.h"
#include "animations/interface.h"
#include "raw_hid.h"

// OS layout state received via Raw HID from host daemon
// 0 = US English (default), 1 = Russian
// NOTE: stays at 0 on machines without the host agent running — any logic
// gated on this will behave as if the OS layout is always US.
uint8_t current_os_layout = 0;

#ifdef LUNA_ENABLE
#include "luna.h"
#endif

#ifdef MATRIX_ENABLE
#include "matrix_gif.h"
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
        KC_TAB,  KC_Q,      KC_1,    MO(NUMBERS), TO(HOGWARTS), KC_F8,
        KC_LCTL, KC_A,      KC_W,       KC_E, KC_R, KC_T,
        KC_LSFT, KC_Z,      KC_S,       KC_D, KC_F, KC_G,
        KC_X,   KC_X, KC_C, KC_V, KC_B,    // <--- first button does not work on hardware level, probably soldering error or TRRS short circuiting
        MO(NAVIGATION),
        LT(SYMBOLS, KC_SPC), KC_ENT,
        ANIM_CYCLE, _______, KC_LALT, KC_ESC,  _______, // first four are d-pad on the left  half
        // KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,  _______,
        _______, _______,                    _______,

        // 右手
        PRTSCR,  HARPOON_1, HARPOON_2,  KC_0,    KC_F8,  KC_F7,
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  SLSH_SLOW,
                                                 MO(NAVIGATION),
        LT(SYMBOLS, KC_BSPC), LANG,
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,   _______, // this is trackball
        _______, _______,                     _______
    ),
    [NAVIGATION] = LAYOUT(
        // 左手
        RGB_TOG, SYM_GATE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
        _______, TMUX_1,  TMUX_2,  TMUX_3,  TMUX_4,  TMUX_5,
        _______, TUMBLER_Z, NAV_TRM, NAV_BRO, NAV_TEL, NAV_XX1,
                 _______, _______, TUMBLER_BRO, TUMBLER_X, TUMBLER_Y,
                          KC_TRNS,
        BRO_LEF, _______,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______,

        // 右手
        _______, _______, _______, _______, _______, _______,
        TMUX_6,  TMUX_7,  TMUX_8,  TMUX_9,  TMUX_10, _______,
        NAV_XX2, NAV_XX3, KC_WBAK, KC_WREF, KC_WFWD, _______,
        KC_F7,   RCS(KC_F11), _______, _______, _______,
                                   KC_TRNS,
        BRO_ADL, BRO_RIG,
        _______, _______, _______, _______,          _______,
        _______, _______,                            _______
    ),
    [SYMBOLS] = LAYOUT(
        // 左手
        // Layer-level switch to US on entry ensures all keycodes produce US symbols
        UC(64), _______, _______,  _______, _______, _______,
        _______, KC_ASTR, KC_EQUAL, KC_BSLS, KC_PIPE, KC_SCLN,
        _______, KC_CIRC, KC_GRV,   KC_DLR,  KC_AT,   KC_EXLM,
                 _______, KC_MINS,  KC_LABK, KC_RABK, S(KC_SLSH), // excl-mark
                          _______,
        _______, _______,
        _______, _______, _______,  _______,          _______,
        _______, _______,                             _______,


        // 右手
        _______, _______, _______, _______, _______, _______,
        _______, KC_LCBR, KC_RCBR, KC_PLUS, KC_AMPR, S(KC_QUOT), // double quote
        KC_TILD, KC_LPRN, KC_RPRN, KC_HASH, S(KC_SCLN), KC_QUOT, // colon
        KC_PERC, KC_LBRC, KC_RBRC, KC_UNDS, KC_SLSH,
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
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  SLSH_SLOW,
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
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  SLSH_SLOW,
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
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  SLSH_SLOW,
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
        KC_LALT,
        KC_SPC, KC_ENT, // first key modified
        KC_6, KC_8, KC_5, KC_7,  _______, // first four are d-pad on the left half, up\down\left\right
        _______, _______,                    _______,


        // 右手
        TO(BASE),  HARPOON_1, HARPOON_2,  KC_0,    KC_F11,  KC_DELETE, // first key modified
        KC_Y,    KC_U,      I_ESC,      KC_O,    KC_P,    LBRC_RCTL,
        KC_H,    KC_J,      KC_K,       KC_L,    KC_SCLN, QUOT_RSFT,
        KC_N,    KC_M,      KC_COMM,    KC_DOT,  SLSH_SLOW,
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

const uint16_t PROGMEM tmux_copy[] = {KC_U, KC_F7, COMBO_END};
const uint16_t PROGMEM tmux_highlight[] = {KC_J, KC_F7, COMBO_END};
const uint16_t PROGMEM tmux_search[] = {KC_M, KC_F7, COMBO_END};

const uint16_t PROGMEM bootloader[] = {KC_Q, KC_W, KC_A, KC_S,  COMBO_END};
const uint16_t PROGMEM bootloader_right[] = {KC_O, KC_L, KC_P, KC_F8,  COMBO_END};

enum combo_index {
    COMBO_MOUSE_LEFT,
    COMBO_MOUSE_RIGHT,
    COMBO_LEFT_GUI,
    COMBO_LEFT_ALT,
    COMBO_TMUX_COPY,
    COMBO_TMUX_HIGHLIGHT,
    COMBO_TMUX_SEARCH,
    COMBO_BOOTLOADER_LEFT,
    COMBO_BOOTLOADER_RIGHT,
};

combo_t key_combos[] = {
    [COMBO_MOUSE_LEFT]       = COMBO(lkm, KC_MS_BTN1),
    [COMBO_MOUSE_RIGHT]      = COMBO(pkm, KC_MS_BTN2),
    [COMBO_LEFT_GUI]         = COMBO(lgui, KC_LGUI),
    [COMBO_LEFT_ALT]         = COMBO(lalt, KC_LALT),

    [COMBO_TMUX_COPY]        = COMBO(tmux_copy, TMUX_COPY),
    [COMBO_TMUX_HIGHLIGHT]   = COMBO(tmux_highlight, TMUX_HIGHLIGHT),
    [COMBO_TMUX_SEARCH]      = COMBO(tmux_search, TMUX_SEARCH),

    [COMBO_BOOTLOADER_LEFT]  = COMBO(bootloader, QK_BOOTLOADER),
    [COMBO_BOOTLOADER_RIGHT] = COMBO(bootloader_right, QK_BOOTLOADER),
};

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    (void)combo;
    (void)keycode;
    (void)record;

    return combo_index != COMBO_LEFT_GUI || tumbler_state.win_combo_enabled;
}

static animation_mode_t current_animation = ANIMATION_UNDERGLOW;

void matrix_scan_user(void) {
    custom_keycodes_matrix_scan();

    // Run RGB layer scanning for underglow animation
    if (current_animation == ANIMATION_UNDERGLOW) {
        rgb_layers_scan(0);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Debug purposes: show which physical key has been pressed
    // uprintf("Key pressed - Row: %d, Col: %d\n", row, col);

    // Trigger animation on any key press FIRST, before custom processing
    // This ensures all physical keypresses are tracked, even if custom keycodes return false
    if (record->event.pressed && keycode != ANIM_CYCLE) {
        update_leds_on_keypress(current_animation, record->event.key.row, record->event.key.col);
    }

    // Handle animation mode cycling
    if (keycode == ANIM_CYCLE && record->event.pressed) {
        // Clear all LEDs before switching to prevent remnants
        clear_all_leds();

        // Cycle to next animation mode
        current_animation = (current_animation + 1) % 2;  // 2 animation modes total

        // Log the change
        switch (current_animation) {
            case ANIMATION_UNDERGLOW:
                uprintf("Switched to UNDERGLOW animation\n");
                break;
            case ANIMATION_SEQUENTIAL:
                uprintf("Switched to SEQUENTIAL animation\n");
                break;
        }

        return false;  // Don't trigger animation update for this keycode
    }

#ifdef LUNA_ENABLE
    luna_process_record(keycode, record);
#endif

    // CKA exam safety: block Ctrl+W to prevent closing browser tabs
    // if (keycode == KC_W && (get_mods() & MOD_MASK_CTRL)) {
    //     return false;
    // }

    // Symbol gate: disabled
    // if (symbol_gate_active && current_os_layout == 0 && get_highest_layer(layer_state) == BASE) {
    //     switch (keycode) {
    //     case KC_1:
    //     case KC_SCLN:
    //         return false;
    //     }
    // }

    // Process custom keycodes
    if (!process_custom_keycodes(keycode, record)) {
        return false;
    }

    return true;
}


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_0;
    } else {
        return rotation;
    }
}


// colors_to_slave_t and layer_to_slave_t are now defined in animations/interface.h

// Slave-side storage for received LED state
static colors_to_slave_t received_led_state = {0};

uint8_t layer_on_slave = 255;  // Initialize to see if it changes

static const char PROGMEM layer_labels[][3] = {
    [BASE]       = {'B', 'A', 'S'},
    [NAVIGATION] = {'N', 'A', 'V'},
    [SYMBOLS]    = {'S', 'Y', 'M'},
    [NUMBERS]    = {'N', 'U', 'M'},
    [SC_MAIN]    = {'S', 'C', '1'},
    [SC_SEC]     = {'S', 'C', '2'},
    [SC_THI]     = {'S', 'C', '3'},
    [HOGWARTS]   = {'H', 'O', 'G'},
};

// Five-by-seven glyphs used by the layer labels. They are drawn at 2x scale
// and rotated clockwise, making the three-letter word fill the 32-pixel edge.
static const uint8_t PROGMEM large_font[36][7] = {
    ['A' - 'A'] = {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    ['B' - 'A'] = {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E},
    ['C' - 'A'] = {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F},
    ['G' - 'A'] = {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F},
    ['H' - 'A'] = {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11},
    ['M' - 'A'] = {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11},
    ['N' - 'A'] = {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11},
    ['O' - 'A'] = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    ['S' - 'A'] = {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E},
    ['U' - 'A'] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    ['V' - 'A'] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04},
    ['Y' - 'A'] = {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04},
    [26 + 1]    = {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E},
    [26 + 2]    = {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F},
    [26 + 3]    = {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E},
};

static void draw_horizontal_line(uint8_t x, uint8_t y, uint8_t width) {
    for (uint8_t dx = 0; dx < width; dx++) {
        oled_write_pixel(x + dx, y, true);
    }
}

static void draw_vertical_line(uint8_t x, uint8_t y, uint8_t height) {
    for (uint8_t dy = 0; dy < height; dy++) {
        oled_write_pixel(x, y + dy, true);
    }
}

static void draw_tumbler(uint8_t x, uint8_t width, bool enabled) {
    const uint8_t y      = 4;
    const uint8_t height = 24;

    draw_horizontal_line(x, y, width);
    draw_horizontal_line(x, y + height - 1, width);
    draw_vertical_line(x, y, height);
    draw_vertical_line(x + width - 1, y, height);

    // A raised, solid upper rocker is on. A diagonally hatched lower rocker is off.
    const uint8_t fill_start = enabled ? y + 2 : y + 13;
    const uint8_t fill_end   = enabled ? y + 10 : y + height - 2;
    for (uint8_t fill_y = fill_start; fill_y < fill_end; fill_y++) {
        for (uint8_t fill_x = 2; fill_x < width - 2; fill_x++) {
            if (enabled || (fill_x + fill_y) % 4 == 0) {
                oled_write_pixel(x + fill_x, fill_y, true);
            }
        }
    }
}

static uint8_t large_glyph_row(char glyph, uint8_t row) {
    uint8_t glyph_index;
    if (glyph >= 'A' && glyph <= 'Z') {
        glyph_index = glyph - 'A';
    } else if (glyph >= '0' && glyph <= '9') {
        glyph_index = 26 + glyph - '0';
    } else {
        return 0;
    }

    return pgm_read_byte(&large_font[glyph_index][row]);
}

static void draw_large_rotated_glyph(uint8_t x, uint8_t y, char glyph) {
    for (uint8_t source_y = 0; source_y < 7; source_y++) {
        const uint8_t pixels = large_glyph_row(glyph, source_y);
        for (uint8_t source_x = 0; source_x < 5; source_x++) {
            if (!(pixels & (1 << (4 - source_x)))) {
                continue;
            }

            const uint8_t rotated_x = x + (6 - source_y) * 2;
            const uint8_t rotated_y = y + source_x * 2;
            for (uint8_t scale_x = 0; scale_x < 2; scale_x++) {
                for (uint8_t scale_y = 0; scale_y < 2; scale_y++) {
                    oled_write_pixel(rotated_x + scale_x, rotated_y + scale_y, true);
                }
            }
        }
    }
}

static void draw_large_layer_label(uint8_t x, uint8_t layer) {
    if (layer >= ARRAY_SIZE(layer_labels)) {
        return;
    }

    for (uint8_t character = 0; character < 3; character++) {
        draw_large_rotated_glyph(
            x,
            1 + character * 10,
            pgm_read_byte(&layer_labels[layer][character])
        );
    }
}

static void render_left_status(void) {
    static uint8_t last_z = 255;
    static uint8_t last_bro = 255;
    static uint8_t last_x = 255;
    static uint8_t last_y = 255;

    if (tumbler_state.win_combo_enabled == last_z &&
        tumbler_state.nav_bro_enabled == last_bro &&
        tumbler_state.nav_tel_enabled == last_x &&
        tumbler_state.nav_xx1_enabled == last_y) {
        return;
    }

    last_z = tumbler_state.win_combo_enabled;
    last_bro = tumbler_state.nav_bro_enabled;
    last_x = tumbler_state.nav_tel_enabled;
    last_y = tumbler_state.nav_xx1_enabled;

    oled_clear();
    draw_tumbler(10, 37, tumbler_state.win_combo_enabled);
    draw_tumbler(53, 21, tumbler_state.nav_bro_enabled);
    draw_tumbler(80, 21, tumbler_state.nav_tel_enabled);
    draw_tumbler(107, 21, tumbler_state.nav_xx1_enabled);
}

#ifdef MATRIX_ENABLE
static void clear_right_label_region(void) {
    for (uint8_t page = 0; page < OLED_DISPLAY_HEIGHT / 8; page++) {
        for (uint8_t x = 0; x < 17; x++) {
            oled_write_raw_byte(0, page * OLED_DISPLAY_WIDTH + x);
        }
    }
}

static void render_right_status(void) {
    static uint8_t last_layer = 255;

    const bool    frame_updated = render_matrix_animation();
    const uint8_t current_layer = get_highest_layer(layer_state);
    if (!frame_updated && current_layer == last_layer) {
        return;
    }

    last_layer = current_layer;
    clear_right_label_region();
    draw_large_layer_label(1, current_layer);
    draw_vertical_line(16, 2, 28);
}
#endif

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

    // Show received LED state on slave
    if (!is_keyboard_master()) {
        char buf[22];
        snprintf(buf, sizeof(buf), "LEDs: %d", received_led_state.led_count);
        oled_write_ln(buf, false);
        for (uint8_t i = 0; i < received_led_state.led_count && i < 2; i++) {
            snprintf(buf, sizeof(buf), "#%d:%d,%d,%d",
                layer_on_slave,
                layer_on_slave,
                layer_on_slave,
                layer_on_slave
                // received_led_state.led_indices[i],
                // received_led_state.led_hsv[i][0],
                // received_led_state.led_hsv[i][1],
                // received_led_state.led_hsv[i][2]
            );
            oled_write_ln(buf, false);
        }
    }
}

bool oled_task_user(void) {
    if (is_keyboard_left()) {
        render_left_status();
        return false;
    }

#ifdef MATRIX_ENABLE
    render_right_status();
#else
    oled_clear();
    draw_large_layer_label(1, get_highest_layer(layer_state));
#endif

    return false;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return TAPPING_TERM;
    }
}


void rpc_animation_slave_step_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen == sizeof(colors_to_slave_t)) {
        const colors_to_slave_t* m2s = (const colors_to_slave_t*)in_data;

        // Apply LED colors received from master (slave will only set its own LEDs)
        for (uint8_t i = 0; i < m2s->led_count && i < 8; i++) {
            rgblight_sethsv_at(
                m2s->led_hsv[i][0],  // H
                m2s->led_hsv[i][1],  // S
                m2s->led_hsv[i][2],  // V
                m2s->led_indices[i]
            );
        }

        // Store locally on slave for OLED display
        memcpy(&received_led_state, in_data, sizeof(colors_to_slave_t));
    }
}


extern uint8_t last_synced_layer;
void layer_refresh_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen == sizeof(layer_to_slave_t)) {
        const layer_to_slave_t* m2s = (const layer_to_slave_t*)in_data;
        layer_on_slave = m2s->layer_idx;
        apply_static_layer_colors(layer_on_slave);
    }
}

void tumbler_state_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    if (in_buflen == sizeof(tumbler_state)) {
        memcpy(&tumbler_state, in_data, sizeof(tumbler_state));
    }
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if (length < 2) return;
    if (data[0] == 0x01) {  // Layout update command
        current_os_layout = data[1];
        uprintf("HID layout update: %d\n", current_os_layout);
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_ANIMATION_STEP, rpc_animation_slave_step_handler);
    transaction_register_rpc(SLAVE_LAYER_REFRESH, layer_refresh_slave_handler);
    transaction_register_rpc(TUMBLER_STATE_SYNC, tumbler_state_sync_handler);
    // rgb_layers_init();
}

void housekeeping_task_user(void) {
    // Run animation housekeeping (includes key flash)
    update_leds_on_housekeeping(current_animation);
}
