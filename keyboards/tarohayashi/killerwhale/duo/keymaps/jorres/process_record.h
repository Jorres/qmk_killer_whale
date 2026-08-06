#pragma once

#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

typedef struct {
    uint8_t win_combo_enabled;
    uint8_t nav_tel_enabled;
    uint8_t nav_xx1_enabled;
} tumbler_state_t;

extern tumbler_state_t tumbler_state;

// Main process record handler - returns true if keycode was not handled
bool process_custom_keycodes(uint16_t keycode, keyrecord_t *record);

// Timer scan functions to be called from matrix_scan_user
void custom_keycodes_matrix_scan(void);

// Symbol gate: when active, symbol keys on BASE are silently blocked
extern bool symbol_gate_active;
