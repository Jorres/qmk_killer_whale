#pragma once

#include QMK_KEYBOARD_H
#include "custom_keycodes.h"

// Main process record handler - returns true if keycode was not handled
bool process_custom_keycodes(uint16_t keycode, keyrecord_t *record);

// Timer scan functions to be called from matrix_scan_user
void custom_keycodes_matrix_scan(void);

// Symbol gate: when active, symbol keys on BASE are silently blocked
extern bool symbol_gate_active;
