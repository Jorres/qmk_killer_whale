// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "lib/add_oled.h"
#include "os_detection.h"
#include "lib/common_killerwhale.h"
#include "lib/glcdfont.c"
#include "lib/add_keycodes.h"

uint8_t pre_layer;
uint8_t cur_layer;
bool interrupted;
uint16_t interrupted_time;
uint16_t interrupt_type;
bool tempch;
uint16_t tempch_time;
uint16_t tempch_type;

// 初期化
void oled_init_addedoled(void){
    pre_layer = 0;
    cur_layer = 0;
    interrupted = false;
    interrupted_time = 0;
    interrupt_type = 0;
    tempch = 0;
    tempch_time = 0;
    tempch_type = 0;
}


// OLED表示
bool oled_task_addedoled(void) {
    if(interrupted){
        if(timer_elapsed(interrupted_time) < INTERRUPT_TIME){
            oled_set_cursor(0, 3);
            switch (interrupt_type){
                case L_CHMOD:
                    if(kw_config.pd_mode_l == SCROLL_MODE){
                        oled_write_P(PSTR("SCROLL MODE LEFT     "), false);
                    }else if(kw_config.pd_mode_l == CURSOR_MODE){
                        oled_write_P(PSTR("CURSOR MODE LEFT     "), false);
                    }
                    break;
                case R_CHMOD:
                    if(kw_config.pd_mode_r == SCROLL_MODE){
                        oled_write_P(PSTR("SCROLL MODE RIGHT    "), false);
                    }else if(kw_config.pd_mode_r == CURSOR_MODE){
                        oled_write_P(PSTR("CURSOR MODE RIGHT    "), false);
                    }
                    break;
                case AUTO_MOUSE:
                    if(kw_config.auto_mouse){
                        oled_write_P(PSTR("AUTO MOUSE ON        "), false);
                    }else{
                        oled_write_P(PSTR("AUTO MOUSE OFF       "), false);
                    }
                    break;
                case OLED_MOD:
                    if(kw_config.oled_mode){
                        oled_write_P(PSTR("SHOW LAYER           "), false);
                    }else{
                        oled_write_P(PSTR("SHOW STATS           "), false);
                    }
                    break;
                case QK_USER_14:
                    if(get_dpad_exclusion()){
                        oled_write_P(PSTR("DPAD EXCLUSION ON    "), false);
                    }else{
                        oled_write_P(PSTR("DPAD EXCLUSION OFF   "), false);
                    }
                    break;
                case QK_USER_15:
                    if(kw_config.rgb_layer){
                        oled_write_P(PSTR("RGBLIGHT LAYER ON    "), false);
                    }else{
                        oled_write_P(PSTR("RGBLIGHT LAYER OFF   "), false);
                    }
                    break;
            }
        }else{
            interrupted = false;
        }

    // 切り替え表示
    }else if(tempch && timer_elapsed(tempch_time) > TERM_TEMP){
        oled_set_cursor(0, 3);
        switch (tempch_type){
            case MOD_SCRL:
            case QK_USER_0:
            case QK_USER_1:
            case QK_USER_2:
            case QK_USER_3:
                oled_write_P(PSTR("SCROLL MODE          "), false);
                break;
            case QK_USER_4:
            case QK_USER_5:
            case QK_USER_6:
            case QK_USER_7:
            case QK_USER_8:
            case QK_USER_9:
            case QK_USER_10:
            case QK_USER_11:
                oled_write_P(PSTR("SLOW MODE            "), false);
                break;
            case QK_USER_12:
                oled_write_P(PSTR("CURSOR MODE          "), false);
                break;
            case QK_USER_13:
                oled_write_P(PSTR("KEY INPUT MODE       "), false);
                break;
        }
    }
    return true;
}

void oled_interrupt(uint16_t keycode){
    interrupted = true;
    interrupted_time = timer_read();
    interrupt_type = keycode;
}

void oled_tempch(bool on, uint16_t keycode){
    tempch = on;
    tempch_time = timer_read();
    tempch_type = keycode;
}
