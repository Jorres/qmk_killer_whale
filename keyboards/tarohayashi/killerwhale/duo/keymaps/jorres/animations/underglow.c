#include "underglow.h"
#include "interface.h"
#include "../rgb_layers.h"

void underglow_on_housekeeping(void) {
    if (!is_keyboard_master()) return;

    uint8_t current_layer = get_highest_layer(layer_state);

    // Sync layer state to slave when layer changes
    if (rgb_layers_task(0)) {
        layer_to_slave_t m2s = {current_layer};
        if (transaction_rpc_send(SLAVE_LAYER_REFRESH, sizeof(m2s), &m2s)) {
            uprintf("triggered layer reload on slave, set to %d\n", current_layer);
        }
    }

    // Corner LED animation (only on base layer)
    static uint32_t last_sync = 0;
    if (timer_elapsed32(last_sync) > BASE_ANIM_INTERVAL && current_layer == 0) {
        colors_to_slave_t m2s = {0};

        m2s.led_count = 8;
        // Get current corner LED colors from master for all 8 LEDs
        for (uint8_t i = 0; i < 8; i++) {
            m2s.led_indices[i] = corner_leds[i];
            // Use corner index (0-3) for both left and right
            get_corner_led_hsv(i,
                &m2s.led_hsv[i][0],
                &m2s.led_hsv[i][1],
                &m2s.led_hsv[i][2]
            );
        }

        if (transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s), &m2s)) {
            last_sync = timer_read32();
        } else {
            uprintf("LED sync failed\n");
        }
    }
}

void underglow_on_keypress(void) {
    // Underglow animation doesn't respond to key presses
}
