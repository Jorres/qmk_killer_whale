#pragma once

#include QMK_KEYBOARD_H

// Luna animation settings
#define MIN_WALK_SPEED      10
#define MIN_RUN_SPEED       40
#define ANIM_FRAME_DURATION 180  // how long each frame lasts in ms
#define ANIM_SIZE           96   // number of bytes in array

// Luna state variables (extern declarations)
extern uint32_t anim_timer;
extern uint8_t current_frame;
extern int current_wpm;
extern led_t led_usb_state;
extern bool isSneaking;
extern bool isJumping;
extern bool showedJump;

// Luna functions
void render_luna(int LUNA_X, int LUNA_Y);
bool luna_process_record(uint16_t keycode, keyrecord_t *record);
