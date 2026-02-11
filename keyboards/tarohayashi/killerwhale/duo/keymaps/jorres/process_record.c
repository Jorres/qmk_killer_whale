#include "process_record.h"
#include "lib/add_keycodes.h"
#include "lib/common_killerwhale.h"
#include "lang_keys.h"

bool symbol_gate_active = true;

extern uint8_t current_os_layout;

// Timer state variables
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

// Called from matrix_scan_user to handle timer-based key behaviors
void custom_keycodes_matrix_scan(void) {
    if (i_esc_pressed && timer_elapsed(i_esc_timer) >= TAPPING_TERM) {
        i_esc_pressed = false;
        tap_code(KC_ESC);
    }
}

bool process_custom_keycodes(uint16_t keycode, keyrecord_t *record) {
    // Handle key interruptions for tap-hold keys
    if (keycode != I_ESC && i_esc_pressed) {
        i_esc_pressed = false;
        tap_code(KC_I);
    }

    if (keycode != LBRC_RCTL && r_ctl_alone) {
        r_ctl_alone = false;
        if (timer_elapsed(r_ctl_timer) < TAPPING_TERM) {
            if (!(symbol_gate_active && current_os_layout == 0 && get_highest_layer(layer_state) == 0)) {
                tap_code(KC_LBRC);
            }
        } else if (!registered_r_ctl) {
            registered_r_ctl = true;
            register_code(KC_RCTL);
        }
    }

    if (keycode != QUOT_RSFT && r_sft_alone) {
        r_sft_alone = false;
        if (timer_elapsed(r_sft_timer) < QUOT_RSFT_TAPPING_TERM) {
            if (!(symbol_gate_active && current_os_layout == 0 && get_highest_layer(layer_state) == 0)) {
                tap_code(KC_QUOT);
            }
        } else if (!registered_r_sft) {
            registered_r_sft = true;
            register_code(KC_RSFT);
        }
    }

    if (keycode != SLSH_SLOW && slsh_alone) {
        slsh_alone = false;
        if (timer_elapsed(slsh_timer) < TAPPING_TERM) {
            tap_code(KC_SLSH);
        } else if (!registered_slsh) {
            registered_slsh = true;
            // No need to register anything, just for consistency
        }
    }

    switch (keycode) {
    case SYM_GATE:
        if (record->event.pressed) {
            symbol_gate_active = !symbol_gate_active;
            uprintf("SYM_GATE: %s\n", symbol_gate_active ? "ON" : "OFF");
        }
        return false;
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
                if (!(symbol_gate_active && current_os_layout == 0 && get_highest_layer(layer_state) == 0)) {
                    tap_code(KC_LBRC);
                }
            }
            registered_r_ctl = false;
            r_ctl_timer = 0;
            r_ctl_alone = false;
        }
        return false;
    case QUOT_RSFT:
        uprintf("QUOT_RSFT\n");
        if (record->event.pressed) {
            set_scroll_mode();
            r_sft_timer = timer_read();
            r_sft_alone = true;
        } else {
            if (registered_r_sft) {
                unregister_code(KC_RSFT);
            } else if (r_sft_alone && timer_elapsed(r_sft_timer) < TAPPING_TERM) {
                if (!(symbol_gate_active && current_os_layout == 0 && get_highest_layer(layer_state) == 0)) {
                    tap_code(KC_QUOT);
                }
            }
            unset_scroll_mode();
            registered_r_sft = false;
            r_sft_timer = 0;
            r_sft_alone = false;
        }
        return false;
    case SLSH_SLOW:
        uprintf("SLSH_SLOW\n");
        if (record->event.pressed) {
            is_slow_mode(true);
            slsh_timer = timer_read();
            slsh_alone = true;
        } else {
            if (registered_slsh) {
                // Nothing to unregister
            } else if (slsh_alone && timer_elapsed(slsh_timer) < TAPPING_TERM) {
                tap_code(KC_SLSH);
            }
            is_slow_mode(false);
            registered_slsh = false;
            slsh_timer = 0;
            slsh_alone = false;
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
    case TMUX_4:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_4);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_4);
        }
        return false;
    case TMUX_5:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_5);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_5);
        }
        return false;
    case TMUX_6:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_6);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_6);
        }
        return false;
    case TMUX_7:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_7);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_7);
        }
        return false;
    case TMUX_8:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_8);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_8);
        }
        return false;
    case TMUX_9:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_9);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_9);
        }
        return false;
    case TMUX_10:
        if (record->event.pressed) {
            register_code(KC_LALT);
            register_code(KC_0);
        } else {
            unregister_code(KC_LALT);
            unregister_code(KC_0);
        }
        return false;
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
    }

    // Delegate language-aware keys to lang_keys.c
    if (process_lang_key(keycode, record)) {
        return false;
    }

    return true;  // Key not handled, continue processing
}
