// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "lib/common_killerwhale.h"
#include "analog.h"
#include "math.h"
#include "os_detection.h"
#include "lib/add_keycodes.h"
#include "lib/add_oled.h"

/* ポインティングデバイス用変数 */
kw_config_t kw_config;         // eeprom保存用
bool force_scrolling;          // 一時的モード変更用
bool force_cursoring;
bool force_key_input;
bool slow_mode;
float scroll_accumulated_l_h;  // スクロール端数保存用
float scroll_accumulated_l_v;
float scroll_accumulated_r_h;
float scroll_accumulated_r_v;
int16_t gp27_newt;              // ジョイスティックの初期値
int16_t gp28_newt;

// 仮想十字キー設定用変数
keypos_t key_up_l;
keypos_t key_up_r;
keypos_t key_left_l;
keypos_t key_left_r;
keypos_t key_right_l;
keypos_t key_right_r;
keypos_t key_down_l;
keypos_t key_down_r;
bool pressed_up_l = false;
bool pressed_up_r = false;
bool pressed_down_l = false;
bool pressed_down_r = false;
bool pressed_left_l = false;
bool pressed_left_r = false;
bool pressed_right_l = false;
bool pressed_right_r = false;
int8_t layer;
int16_t keycode_up_l;
int16_t keycode_up_r;
int16_t keycode_down_l;
int16_t keycode_down_r;
int16_t keycode_left_l;
int16_t keycode_left_r;
int16_t keycode_right_l;
int16_t keycode_right_r;
int16_t key_timer_l;
int16_t key_timer_r;

// 斜め入力防止用変数
bool dpad_exclusion;
uint8_t dpad_pressed_l;
uint8_t dpad_pressed_r;

/* eeprom */
// 初期化
void eeconfig_init_kb(void) {
    kw_config.spd_l = SPD_DEFAULT_LEFT;
    kw_config.spd_r = SPD_DEFAULT_RIGHT;
    kw_config.angle_l = ANGLE_DEFAULT_LEFT;
    kw_config.angle_r = ANGLE_DEFAULT_RIGHT;
    kw_config.pd_mode_l = MODE_DEFAULT_LEFT;
    kw_config.pd_mode_r = MODE_DEFAULT_RIGHT;
    kw_config.inv_l = INVERT_LEFT_DEFAULT;
    kw_config.inv_r = INVERT_RIGHT_DEFAULT;
    kw_config.inv_sc = INVERT_SCROLL_DEFAULT;
    kw_config.auto_mouse = AUTO_MOUSE_DEFAULT;
    kw_config.rgb_layer = RGB_LAYER_DEFAULT;
    kw_config.oled_mode = OLED_DEFAULT;
    eeconfig_update_kb(kw_config.raw);

    eeconfig_init_user();
}

/* キースキャン */
// マウスキー登録
// bool is_mouse_record_kb(uint16_t keycode, keyrecord_t* record) {
//     switch(keycode) {
//         case MOD_SCRL: // 一時的にスクロール
//             return true;
//         case QK_USER_4: // 一時的に減速
//             return true;
//         case QK_USER_9: // マウス1 / 長押しスロー
//             return true;
//         case QK_USER_10: // マウス2 / 長押しスロー
//             return true;
//         case QK_USER_11: // マウス3 / 長押しスロー
//             return true;
//         case QK_USER_12: // 一時的にカーソル移動
//             return true;
//         default:
//             return false;
//     }

//     return  is_mouse_record_user(keycode, record);
// }


// bool is_mouse_record_user(uint16_t keycode, keyrecord_t* record) {
//     return false;
// }

// 実タスク
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    keypos_t key = record->event.key;
    uprintf("Pressed key at row: %d col: %d, value = %d\n", key.row, key.col, keycode);

    // 追加キーコードタスク
    bool res = process_record_addedkeycodes(keycode, record);
    if (!res) {
        return false;
    }

    // D-Padの同時押しを防ぐ
    // keypos_t key = record->event.key;
    // if(key.col == 6 && dpad_exclusion){
    //     switch (key.row){
    //         case 1:
    //         case 2:
    //         case 3:
    //         case 4:
    //             if(dpad_pressed_l == 0 && record->event.pressed){
    //                 if(record->event.pressed){
    //                     dpad_pressed_l  = key.row;
    //                 }
    //             }else if(dpad_pressed_l == key.row && !record->event.pressed){
    //                 dpad_pressed_l  = 0;
    //             }else if(record->event.pressed){
    //                 return false;
    //             }
    //             break;
    //         case 8:
    //         case 9:
    //         case 10:
    //         case 11:
    //             if(dpad_pressed_r == 0 && record->event.pressed){
    //                 if(record->event.pressed){
    //                     dpad_pressed_r = key.row;
    //                 }
    //             }else if(dpad_pressed_r == key.row && !record->event.pressed){
    //                 dpad_pressed_r = 0;
    //             }else if(record->event.pressed){
    //                 return false;
    //             }
    //             break;
    //     }
    // }

    return process_record_user(keycode, record);
}

/* マトリクス走査 */
// 初期化
void matrix_init_kb(void) {
    key_up_l.row = 3;
    key_up_l.col = 6;
    key_down_l.row = 1;
    key_down_l.col = 6;
    key_left_l.row = 2;
    key_left_l.col = 6;
    key_right_l.row = 4;
    key_right_l.col = 6;
    key_up_r.row = 11;
    key_up_r.col = 6;
    key_down_r.row = 9;
    key_down_r.col = 6;
    key_left_r.row = 10;
    key_left_r.col = 6;
    key_right_r.row = 8;
    key_right_r.col = 6;
    dpad_pressed_l = 0;
    dpad_pressed_r = 0;
    force_scrolling = false;
    force_cursoring = false;
    force_key_input = false;
    slow_mode = false;

    gp27_newt = analogReadPin(GP27);
    gp28_newt = analogReadPin(GP28);
    key_timer_l = timer_read();
    key_timer_r = timer_read();
    dpad_exclusion = DPAD_EX_DEFAULT;

    matrix_init_user();
}

/* カーソル操作 */
// 初期化
void pointing_device_init_kb(void){
    kw_config.raw = eeconfig_read_kb();
    scroll_accumulated_l_h = 0;
    scroll_accumulated_l_v = 0;
    scroll_accumulated_r_h = 0;
    scroll_accumulated_r_v = 0;
    pointing_device_set_cpi_on_side(true,  600 + kw_config.spd_l * 300);
    pointing_device_set_cpi_on_side(false, 600 + kw_config.spd_r * 300);
    // set_auto_mouse_enable(kw_config.auto_mouse);

    pointing_device_init_user();
}
// 実タスク
report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t right_report) {
    // マウスの数値はそのまま使う
    double x_val_l = 0.0;
    double y_val_l = 0.0;
    double x_val_r = 0.0;
    double y_val_r = 0.0;
    x_val_l = (double)left_report.x;
    y_val_l = (double)left_report.y;
    x_val_r = (double)right_report.x;
    y_val_r = (double)right_report.y;
    // 角度補正
    double rad = (double)kw_config.angle_l * 12.0 * (M_PI / 180.0) * -1.0;
    double x_rev_l =  + x_val_l * cos(rad) - y_val_l * sin(rad);
    double y_rev_l =  + x_val_l * sin(rad) + y_val_l * cos(rad);
    rad = (double)kw_config.angle_r * 12.0 * (M_PI / 180.0) * -1.0;
    double x_rev_r =  + x_val_r * cos(rad) - y_val_r * sin(rad);
    double y_rev_r =  + x_val_r * sin(rad) + y_val_r * cos(rad);

    // x軸反転処理
    if(kw_config.inv_l){ x_rev_l = -1.0 * x_rev_l; }
    if(kw_config.inv_r){ x_rev_r = -1.0 * x_rev_r; }

    /* 左手処理 */
    // スクロール処理
    if(!force_key_input && (force_scrolling || kw_config.pd_mode_l == SCROLL_MODE)){
        // 斜め成分を消す
        if (abs(x_rev_l) > abs(y_rev_l)) {
            y_rev_l = 0;
        } else {
            x_rev_l = 0;
        }
        // スクロール反転処理
        if(!kw_config.inv_sc ){
            x_rev_l = -1.0 * x_rev_l;
            y_rev_l = -1.0 * y_rev_l;
        }
        // 端数処理
        scroll_accumulated_l_h += (float)x_rev_l / SCROLL_DIVISOR;
        scroll_accumulated_l_v += (float)y_rev_l / SCROLL_DIVISOR;
        left_report.h = (int8_t)scroll_accumulated_l_h;
        left_report.v = (int8_t)scroll_accumulated_l_v;
        scroll_accumulated_l_h -= (int8_t)scroll_accumulated_l_h;
        scroll_accumulated_l_v -= (int8_t)scroll_accumulated_l_v;

        left_report.x = 0;
        left_report.y = 0;
    // カーソル移動処理
    }else if(!force_key_input && (force_cursoring || kw_config.pd_mode_l == CURSOR_MODE)){
        left_report.x = (int8_t)x_rev_l;
        left_report.y = (int8_t)y_rev_l;
        left_report.h = 0;
        left_report.v = 0;
    // キー入力処理
    }else if(force_key_input){
        // 入力キーの座標指定
        left_report.x = 0;
        left_report.y = 0;
        left_report.h = 0;
        left_report.v = 0;
        layer = layer_switch_get_layer(key_up_l);
        keycode_up_l = keymap_key_to_keycode(layer, key_up_l);
        keycode_left_l = keymap_key_to_keycode(layer, key_left_l);
        keycode_right_l = keymap_key_to_keycode(layer, key_right_l);
        keycode_down_l = keymap_key_to_keycode(layer, key_down_l);

        // 斜めを除外
        if(dpad_exclusion){
            if (abs(x_rev_l) > abs(y_rev_l)) {
                y_rev_l = 0;
            } else {
                x_rev_l = 0;
            }
        }

        // トラックボール時は単入力を一定時間間隔
        if(timer_elapsed(key_timer_l) > TIMEOUT_KEY){
            if((int16_t)x_rev_l > KEY_OFFSET){
                tap_code16(keycode_right_l);
            }else if((int16_t)x_rev_l  < -KEY_OFFSET){
                tap_code16(keycode_left_l);
            }

            if((int16_t)y_rev_l > KEY_OFFSET){
                tap_code16(keycode_down_l);
            }else if((int16_t)y_rev_l < -KEY_OFFSET){
                tap_code16(keycode_up_l);
            }

            key_timer_l = timer_read();
        }
    }

    /* 右手処理 */
    // スクロール処理
    if(!force_key_input && (force_scrolling || kw_config.pd_mode_r == SCROLL_MODE)){
        // 斜め成分を消す
        if (abs(x_rev_r) > abs(y_rev_r)) {
            y_rev_r = 0;
        } else {
            x_rev_r = 0;
        }
        // スクロール反転処理
        if(!kw_config.inv_sc ){
            x_rev_r = -1.0 * x_rev_r;
            y_rev_r = -1.0 * y_rev_r;
        }
        // 端数処理
        scroll_accumulated_r_h += (float)x_rev_r / SCROLL_DIVISOR;
        scroll_accumulated_r_v += (float)y_rev_r / SCROLL_DIVISOR;
        right_report.h = (int8_t)scroll_accumulated_r_h;
        right_report.v = (int8_t)scroll_accumulated_r_v;
        scroll_accumulated_r_h -= (int8_t)scroll_accumulated_r_h;
        scroll_accumulated_r_v -= (int8_t)scroll_accumulated_r_v;

        right_report.x = 0;
        right_report.y = 0;
    // カーソル移動処理}
    }else if(!force_key_input && (force_cursoring || kw_config.pd_mode_r == CURSOR_MODE)){
        right_report.x = (int8_t)x_rev_r;
        right_report.y = (int8_t)y_rev_r;
        right_report.h = 0;
        right_report.v = 0;
    // キー入力処理
    }else if(force_key_input){
        // 入力キーの座標指定
        right_report.x = 0;
        right_report.y = 0;
        right_report.h = 0;
        right_report.v = 0;
        layer = layer_switch_get_layer(key_up_r);
        keycode_up_r = keymap_key_to_keycode(layer, key_up_r);
        keycode_left_r = keymap_key_to_keycode(layer, key_left_r);
        keycode_right_r = keymap_key_to_keycode(layer, key_right_r);
        keycode_down_r = keymap_key_to_keycode(layer, key_down_r);

        // 斜めを除外
        if(dpad_exclusion){
            if (abs(x_rev_r) > abs(y_rev_r)) {
                y_rev_r = 0;
            } else {
                x_rev_r = 0;
            }
        }

        // トラックボール時は単入力を一定時間間隔
        if(timer_elapsed(key_timer_r) > TIMEOUT_KEY){
            if((int16_t)x_rev_r > KEY_OFFSET){
                tap_code16(keycode_right_r);
            }else if((int16_t)x_rev_r  < -KEY_OFFSET){
                tap_code16(keycode_left_r);
            }

            if((int16_t)y_rev_r > KEY_OFFSET){
                tap_code16(keycode_down_r);
            }else if((int16_t)y_rev_r < -KEY_OFFSET){
                tap_code16(keycode_up_r);
            }

            key_timer_r = timer_read();
        }
    }

    return pointing_device_combine_reports(left_report, right_report);
}

/* OLED */
// 初期化
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    // 追加OLED初期化
    oled_init_addedoled();

    return oled_init_user(rotation);
}
// 実タスク
bool oled_task_kb(void) {
    // 追加OLEDタスク
    oled_task_addedoled();

    return oled_task_user();
}

/* 諸関数 */
// モードチェンジ時端数削除
void clear_keyinput(void){
    unregister_code(keycode_up_l);
    unregister_code(keycode_down_l);
    unregister_code(keycode_left_l);
    unregister_code(keycode_right_l);
    scroll_accumulated_l_v = 0;
    scroll_accumulated_l_h = 0;
    unregister_code(keycode_up_r);
    unregister_code(keycode_down_r);
    unregister_code(keycode_left_r);
    unregister_code(keycode_right_r);
    scroll_accumulated_r_v = 0;
    scroll_accumulated_r_h = 0;
}
/* インターフェース */
// モード変更
void cycle_mode_l(void){
    kw_config.pd_mode_l = (kw_config.pd_mode_l + 1) % 2;
    // eeconfig_update_kb(kw_config.raw);
    clear_keyinput();
}
void cycle_mode_r(void){
    kw_config.pd_mode_r = (kw_config.pd_mode_r + 1) % 2;
    // eeconfig_update_kb(kw_config.raw);
    clear_keyinput();
}
// 一時的モード変更
void is_scroll_mode(bool is_force_scrolling){
    force_scrolling = is_force_scrolling;
    clear_keyinput();
}
void set_scroll_mode(void){
    force_scrolling = true;
    clear_keyinput();
}
void unset_scroll_mode(void){
    force_scrolling = false;
    clear_keyinput();
}
void is_cursor_mode(bool is_force_cursoring){
    force_cursoring = is_force_cursoring;
    clear_keyinput();
}
void is_key_mode(bool is_force_key_input){
    force_key_input = is_force_key_input;
    clear_keyinput();
}
void is_slow_mode(bool is_slow_mode){
    slow_mode = is_slow_mode;
    if(is_slow_mode){
        pointing_device_set_cpi_on_side(true,  CPI_SLOW);
        pointing_device_set_cpi_on_side(false, CPI_SLOW);
    }else{
        pointing_device_set_cpi_on_side(true,  600 + kw_config.spd_l * 300);
        pointing_device_set_cpi_on_side(false, 600 + kw_config.spd_r * 300);
    }
    clear_keyinput();
}

bool get_dpad_exclusion(void){
    return dpad_exclusion;
}
void toggle_dpad_exclusion(void){
    dpad_exclusion = !dpad_exclusion;
}
