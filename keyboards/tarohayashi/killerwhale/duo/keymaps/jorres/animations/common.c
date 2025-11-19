#include "interface.h"
#include "underglow.h"
#include "sequential.h"

void update_leds_on_housekeeping(animation_mode_t mode) {
    switch (mode) {
        case ANIMATION_UNDERGLOW:
            underglow_on_housekeeping();
            break;
        case ANIMATION_SEQUENTIAL:
            sequential_on_housekeeping();
            break;
    }
}

void update_leds_on_keypress(animation_mode_t mode) {
    switch (mode) {
        case ANIMATION_UNDERGLOW:
            underglow_on_keypress();
            break;
        case ANIMATION_SEQUENTIAL:
            sequential_on_keypress();
            break;
    }
}

void clear_all_leds(void) {
    if (!is_keyboard_master()) return;

    // Determine which LEDs are on master vs slave
    uint8_t master_start, master_end, slave_start, slave_end;

    if (is_keyboard_left()) {
        // Master is left: 0-32 are master, 33-65 are slave
        master_start = 0;
        master_end = 32;
        slave_start = 33;
        slave_end = 65;
    } else {
        // Master is right: 33-65 are master, 0-32 are slave
        master_start = 33;
        master_end = 65;
        slave_start = 0;
        slave_end = 32;
    }

    // Clear all master LEDs locally
    for (uint8_t i = master_start; i <= master_end; i++) {
        rgblight_sethsv_at(0, 0, 0, i);
    }

    // Clear all slave LEDs via RPC (in batches of 8)
    uint8_t total_slave_leds = slave_end - slave_start + 1;
    for (uint8_t batch_start = 0; batch_start < total_slave_leds; batch_start += 8) {
        colors_to_slave_t m2s = {0};
        m2s.led_count = 0;

        for (uint8_t i = 0; i < 8 && (batch_start + i) < total_slave_leds; i++) {
            m2s.led_indices[m2s.led_count] = slave_start + batch_start + i;
            m2s.led_hsv[m2s.led_count][0] = 0;  // H
            m2s.led_hsv[m2s.led_count][1] = 0;  // S
            m2s.led_hsv[m2s.led_count][2] = 0;  // V
            m2s.led_count++;
        }

        if (m2s.led_count > 0) {
            transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s), &m2s);
        }
    }

    uprintf("Cleared all LEDs\n");
}
