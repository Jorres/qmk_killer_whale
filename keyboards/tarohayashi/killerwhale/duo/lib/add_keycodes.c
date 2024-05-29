// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "lib/add_keycodes.h"
#include "os_detection.h"
#include "lib/common_killerwhale.h"
#include "lib/add_oled.h"


uint16_t startup_timer;
bool process_record_addedkeycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case CMD_CTL:
            uprintf("CMD_CTL\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                }
            } else {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    unregister_code(KC_LGUI);
                } else {
                    unregister_code(KC_LCTL);
                }
            }
            oled_tempch(record->event.pressed, keycode);
            return false;
            break;
        case UNDO:
            uprintf("UNDO\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_Z);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_Z);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case REDO:
            uprintf("REDO\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_Z);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LCTL);
                    tap_code(KC_Z);
                    unregister_code(KC_LCTL);
                    unregister_code(KC_LSFT);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case COPY:
            uprintf("COPY\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_C);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_C);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case CUT:
            uprintf("CUT\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_X);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_X);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case PASTE:
            uprintf("PASTE\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_V);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_V);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case SC_UP:
            uprintf("SC_UP\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_PPLS);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_PPLS);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case SC_DOWN:
            uprintf("SC_DOWN\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_PMNS);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_PMNS);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case SC_RESET:
            uprintf("SC_RESET\n");
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_0);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_0);
                    unregister_code(KC_LCTL);
                }
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_SPD_I:
            uprintf("L_SPD_I\n");
            if (record->event.pressed) {
                kw_config.spd_l = kw_config.spd_l + 1;
                if(kw_config.spd_l >= SPD_OPTION_MAX){
                    kw_config.spd_l = SPD_OPTION_MAX-1;
                }
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(true,  600 + kw_config.spd_l * 300);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_SPD_I:
            uprintf("R_SPD_I\n");
            if (record->event.pressed) {
                kw_config.spd_r = kw_config.spd_r + 1;
                if(kw_config.spd_r >= SPD_OPTION_MAX){
                    kw_config.spd_r = SPD_OPTION_MAX-1;
                }
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(false, 600 + kw_config.spd_r * 300);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_SPD_D:
            uprintf("L_SPD_D\n");
            if (record->event.pressed) {
                if(kw_config.spd_l > 0){
                    kw_config.spd_l = kw_config.spd_l - 1;
                }
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(true,  600 + kw_config.spd_l * 300);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_SPD_D:
            uprintf("R_SPD_D\n");
            if (record->event.pressed) {
                if(kw_config.spd_r > 0){
                    kw_config.spd_r = kw_config.spd_r - 1;
                }
                eeconfig_update_kb(kw_config.raw);
                pointing_device_set_cpi_on_side(false, 600 + kw_config.spd_r * 300);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_ANG_I:
            uprintf("L_ANG_I\n");
            if (record->event.pressed) {
                kw_config.angle_l = (kw_config.angle_l + 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_ANG_I:
            uprintf("R_ANG_I\n");
            if (record->event.pressed) {
                kw_config.angle_r = (kw_config.angle_r + 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_ANG_D:
            uprintf("L_ANG_D\n");
            if (record->event.pressed) {
                kw_config.angle_l = (ANGLE_OPTION_MAX + kw_config.angle_l - 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_ANG_D:
            uprintf("R_ANG_D\n");
            if (record->event.pressed) {
                kw_config.angle_r = (ANGLE_OPTION_MAX + kw_config.angle_r - 1) % ANGLE_OPTION_MAX;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_INV:
            uprintf("L_INV\n");
            if (record->event.pressed) {
                kw_config.inv_l = !kw_config.inv_l;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_INV:
            uprintf("R_INV\n");
            if (record->event.pressed) {
                kw_config.inv_r = !kw_config.inv_r;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case INV_SCRL:
            uprintf("INV_SCRL\n");
            if (record->event.pressed) {
                kw_config.inv_sc = !kw_config.inv_sc;
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case L_CHMOD:
            uprintf("L_CHMOD\n");
            if (record->event.pressed) {
                cycle_mode_l();
                oled_interrupt(keycode);
            }
            return false;
            break;
        case R_CHMOD:
            uprintf("R_CHMOD\n");
            if (record->event.pressed) {
                cycle_mode_r();
                oled_interrupt(keycode);
            }
            return false;
            break;
        case AUTO_MOUSE:
            uprintf("AUTO_MOUSE\n");
            if (record->event.pressed) {
                kw_config.auto_mouse = !kw_config.auto_mouse;
                set_auto_mouse_enable(kw_config.auto_mouse);
                eeconfig_update_kb(kw_config.raw);
                oled_interrupt(keycode);
            }
            return false;
            break;
        case OLED_MOD:
            uprintf("OLED_MOD\n");
            if (record->event.pressed) {
                kw_config.oled_mode = !kw_config.oled_mode;
                oled_clear();
                oled_interrupt(keycode);
            }
            return false;
            break;
        case MOD_SCRL:
            uprintf("MOD_SCRL\n");
            is_scroll_mode(record->event.pressed);
            oled_tempch(record->event.pressed, keycode);
            return false;
            break;
    }
    if (record->event.pressed) {
        oled_interrupt(keycode);
    }
    return true;
}
