#include "underglow.h"
#include "interface.h"
#include "../rgb_layers.h"
#include "../led_map.h"

// Key flash LED configuration
#define KEY_FLASH_DURATION 500  // milliseconds
#define MAX_FLASH_LEDS 10  // Track up to 10 simultaneous flashes
#define KEY_FLASH_BRIGHTNESS 128  // Reduced brightness for key flashes (0-255)

// LED flash tracking
typedef struct {
    uint8_t led_index;
    uint32_t start_time;
    bool active;
} led_flash_t;

static led_flash_t flash_leds[MAX_FLASH_LEDS] = {0};

// Check if LED is on master side
static bool is_led_on_master(uint8_t led_index) {
    if (is_keyboard_left()) {
        return led_index <= 32;  // Left master: LEDs 0-32
    } else {
        return led_index >= 33;  // Right master: LEDs 33-65
    }
}

// Handle key flash LED flash (called on keypress)
static void handle_key_flash(uint8_t row, uint8_t col) {
    // Look up LED index for this key
    uint8_t led_index = pgm_read_byte(&matrix_to_led[row][col]);

    if (led_index == 255) {
        return;  // No LED for this key
    }

    // Pick a random color from the shared palette, skipping black colors
    uint8_t color_idx;
    uint8_t h, s, v;
    do {
        color_idx = rand() % UNDERGLOW_COLOR_PALETTE_SIZE;
        h = underglow_color_palette[color_idx][0];
        s = underglow_color_palette[color_idx][1];
        v = underglow_color_palette[color_idx][2];
    } while (v == 0);  // Skip black colors (V=0)

    v = KEY_FLASH_BRIGHTNESS;  // Apply reduced brightness

    // Find a free slot to track this flash
    for (uint8_t i = 0; i < MAX_FLASH_LEDS; i++) {
        if (!flash_leds[i].active) {
            flash_leds[i].led_index = led_index;
            flash_leds[i].start_time = timer_read32();
            flash_leds[i].active = true;
            break;
        }
    }

    // Light up the LED
    if (is_led_on_master(led_index)) {
        // Master side - light directly
        rgblight_sethsv_at(h, s, v, led_index);
    } else {
        // Slave side - send RPC (separate from animation RPCs)
        colors_to_slave_t m2s = {0};
        m2s.led_count = 1;
        m2s.led_indices[0] = led_index;
        m2s.led_hsv[0][0] = h;
        m2s.led_hsv[0][1] = s;
        m2s.led_hsv[0][2] = v;

        transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s), &m2s);
    }
}

// Process key flash timeouts (called in housekeeping)
static void process_key_flash_timeouts(void) {
    for (uint8_t i = 0; i < MAX_FLASH_LEDS; i++) {
        if (flash_leds[i].active) {
            // Check if flash duration has elapsed
            if (timer_elapsed32(flash_leds[i].start_time) > KEY_FLASH_DURATION) {
                uint8_t led_index = flash_leds[i].led_index;

                // Turn off the LED
                if (is_led_on_master(led_index)) {
                    // Master side - turn off directly
                    rgblight_sethsv_at(0, 0, 0, led_index);
                } else {
                    // Slave side - send RPC to turn off
                    colors_to_slave_t m2s = {0};
                    m2s.led_count = 1;
                    m2s.led_indices[0] = led_index;
                    m2s.led_hsv[0][0] = 0;
                    m2s.led_hsv[0][1] = 0;
                    m2s.led_hsv[0][2] = 0;

                    transaction_rpc_send(RPC_ANIMATION_STEP, sizeof(m2s), &m2s);
                }

                flash_leds[i].active = false;
            }
        }
    }
}

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

    // Process key flash LED timeouts
    process_key_flash_timeouts();
}

void underglow_on_keypress(uint8_t row, uint8_t col) {
    // uprintf("Key pressed - Row: %d, Col: %d\n", row, col);

    // Handle key flash LED on keypress
    handle_key_flash(row, col);
}
