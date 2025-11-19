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

    HARPOON_1,
    HARPOON_2,

    CTLSHFT,

    ANIM_CYCLE,  // Cycle through animation modes
};
