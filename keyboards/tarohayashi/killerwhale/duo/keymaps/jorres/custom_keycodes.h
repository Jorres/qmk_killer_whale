#pragma once

#include QMK_KEYBOARD_H

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
    SLSH_SLOW,

    HARPOON_1,
    HARPOON_2,

    CTLSHFT,

    ANIM_CYCLE,  // Cycle through animation modes
    SYM_GATE,    // Toggle: block symbol keys on BASE layer (training mode)
    TUMBLER_Z,   // Toggle the KC_Z + KC_S Win-key combo
    TUMBLER_BRO, // Toggle NAV_BRO
    TUMBLER_X,   // Toggle NAV_TEL
    TUMBLER_Y,   // Toggle NAV_XX1

    // Language-aware keys: contiguous range, processed by lang_keys.c
    // Keys that produce the correct symbol regardless of OS layout (US/RU).
    LANG_KEY_START,
    QUES_LANG = LANG_KEY_START, // ?
    PIPE_LANG,  // |
    CIRC_LANG,  // ^
    GRV_LANG,   // `
    DLR_LANG,   // $
    AT_LANG,    // @
    LABK_LANG,  // <
    RABK_LANG,  // >
    LCBR_LANG,  // {
    RCBR_LANG,  // }
    TILD_LANG,  // ~
    HASH_LANG,  // #
    AMPR_LANG,  // &
    LBRC_LANG,  // [
    RBRC_LANG,  // ]
    LANG_KEY_END = RBRC_LANG,
};
