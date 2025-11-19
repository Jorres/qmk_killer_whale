#include "sequential.h"
#include "interface.h"

// Animation state
static uint8_t current_led = 0;
static uint8_t previous_led = 255;  // 255 means no previous LED yet

// Helper function to determine if an LED is on the master side
static bool is_led_on_master(uint8_t led_index) {
    if (is_keyboard_left()) {
        // Master is left half: LEDs 0-32 are on master
        return led_index < 33;
    } else {
        // Master is right half: LEDs 33-65 are on master
        return led_index >= 33;
    }
}

void sequential_on_housekeeping(void) {
    // Sequential animation doesn't use housekeeping
}

void sequential_on_keypress(void) {
    if (!is_keyboard_master()) return;

    // Turn off the previous LED
    if (previous_led != 255) {
        if (is_led_on_master(previous_led)) {
            // Previous LED is on master - set it directly
            rgblight_sethsv_at(0, 0, 0, previous_led);
        } else {
            // Previous LED is on slave - send RPC
            colors_to_slave_t m2s_clear = {0};
            m2s_clear.led_count = 1;
            m2s_clear.led_indices[0] = previous_led;
            m2s_clear.led_hsv[0][0] = 0;  // H
            m2s_clear.led_hsv[0][1] = 0;  // S
            m2s_clear.led_hsv[0][2] = 0;  // V
            transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s_clear), &m2s_clear);
        }
    }

    // Light up the current LED
    if (is_led_on_master(current_led)) {
        // Current LED is on master - set it directly
        rgblight_sethsv_at(170, 255, 255, current_led);  // Cyan color
    } else {
        // Current LED is on slave - send RPC
        colors_to_slave_t m2s = {0};
        m2s.led_count = 1;
        m2s.led_indices[0] = current_led;
        m2s.led_hsv[0][0] = 170;  // H (cyan)
        m2s.led_hsv[0][1] = 255;  // S
        m2s.led_hsv[0][2] = 255;  // V

        if (transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s), &m2s)) {
            uprintf("Lit LED %d on slave\n", current_led);
        } else {
            uprintf("LED sync failed for LED %d\n", current_led);
        }
    }

    // Update LED positions
    previous_led = current_led;
    current_led++;
    if (current_led >= 66) {
        current_led = 0;
    }
}
