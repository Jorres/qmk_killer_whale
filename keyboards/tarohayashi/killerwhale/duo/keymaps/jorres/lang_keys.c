#include "lang_keys.h"

extern uint8_t current_os_layout;

#define LANG_KEY_COUNT (LANG_KEY_END - LANG_KEY_START + 1)
#define SWITCH_DELAY_MS 50

// US keycodes for each lang key, indexed by (keycode - LANG_KEY_START).
// In Russian mode, we temporarily switch to US, type the key, then switch back.
static const uint16_t us_keycodes[] = {
    [QUES_LANG - LANG_KEY_START] = S(KC_SLSH), // ?
    [PIPE_LANG - LANG_KEY_START] = S(KC_BSLS), // |
    [CIRC_LANG - LANG_KEY_START] = S(KC_6),    // ^
    [GRV_LANG  - LANG_KEY_START] = KC_GRV,     // `
    [DLR_LANG  - LANG_KEY_START] = S(KC_4),    // $
    [AT_LANG   - LANG_KEY_START] = S(KC_2),    // @
    [LABK_LANG - LANG_KEY_START] = S(KC_COMM), // <
    [RABK_LANG - LANG_KEY_START] = S(KC_DOT),  // >
    [LCBR_LANG - LANG_KEY_START] = S(KC_LBRC), // {
    [RCBR_LANG - LANG_KEY_START] = S(KC_RBRC), // }
    [TILD_LANG - LANG_KEY_START] = S(KC_GRV),  // ~
    [HASH_LANG - LANG_KEY_START] = S(KC_3),    // #
    [AMPR_LANG - LANG_KEY_START] = S(KC_7),    // &
    [LBRC_LANG - LANG_KEY_START] = KC_LBRC,    // [
    [RBRC_LANG - LANG_KEY_START] = KC_RBRC,    // ]
};

// Per-key state
static uint16_t active_kc[LANG_KEY_COUNT];
static bool switched_layout[LANG_KEY_COUNT];

static void tap_lang_switch(void) {
    tap_code16(LGUI(KC_SPC));
}

bool process_lang_key(uint16_t keycode, keyrecord_t *record) {
    if (keycode < LANG_KEY_START || keycode > LANG_KEY_END) {
        return false;
    }

    uint8_t idx = keycode - LANG_KEY_START;

    if (record->event.pressed) {
        active_kc[idx] = us_keycodes[idx];
        if (current_os_layout == 1) {
            // Switch to US temporarily, type the symbol, switch back on release
            switched_layout[idx] = true;
            tap_lang_switch();
            wait_ms(SWITCH_DELAY_MS);
        } else {
            switched_layout[idx] = false;
        }
        register_code16(active_kc[idx]);
    } else {
        unregister_code16(active_kc[idx]);
        if (switched_layout[idx]) {
            switched_layout[idx] = false;
            wait_ms(SWITCH_DELAY_MS);
            tap_lang_switch();
        }
    }
    return true;
}
