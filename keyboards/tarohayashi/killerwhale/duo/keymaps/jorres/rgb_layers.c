#include "rgb_layers.h"

// Global state
bool rgb_animation_active = false;
uint16_t base_anim_timer = 0;

// Corner LED indices - global addressing (left: 0-32, right: 33-65)
const uint8_t corner_leds[] = {0, 1, 2, 27, 33, 34, 35, 60};

// Track if RGB needs update (for slave sync)
static uint8_t last_synced_layer = 255;

// Color palette for base layer animation
static const uint8_t base_anim_colors[][5] = {
    {HSV_WHITE},
    {HSV_CYAN},
    {HSV_BLACK},
    {HSV_ORANGE},
    {HSV_BLACK}
};
#define BASE_ANIM_COLOR_COUNT 5
// Current color index for each corner LED
static uint8_t corner_led_colors[CORNER_LED_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0};

// Layer color definitions (HSV for corner LEDs)
static const uint8_t layer_corner_colors[][3] = {
    {HSV_WHITE},   // BASE (0)
    {HSV_YELLOW},  // NAVIGATION (1)
    {HSV_WHITE},   // SYMBOLS (2)
    {HSV_AZURE},   // NUMBERS (3)
    {HSV_WHITE},   // SC_MAIN (4)
    {HSV_AZURE},   // SC_SEC (5)
    {HSV_WHITE},   // SC_THI (6)
    {HSV_CORAL},   // HOGWARTS (7)
};

// Track current layer for change detection
static uint8_t current_rgb_layer = 0;

// Helper function to apply all LED colors for BOTH halves
static void apply_base_layer_leds(void) {
    for (int i = 0; i < CORNER_LED_COUNT; i++) {
        uint8_t color_idx = corner_led_colors[i];
        uint8_t h = base_anim_colors[color_idx][0];
        uint8_t s = base_anim_colors[color_idx][1];
        uint8_t v = base_anim_colors[color_idx][2];
        rgblight_sethsv_at(h, s, v, corner_leds[i]);
    }
}

// Helper function to apply static layer colors (non-BASE layers)
void apply_static_layer_colors(uint8_t layer_idx) {
    if (layer_idx > 7) layer_idx = 0;

    rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);

    uint8_t h = layer_corner_colors[layer_idx][0];
    uint8_t s = layer_corner_colors[layer_idx][1];
    uint8_t v = layer_corner_colors[layer_idx][2];

    for (int i = 0; i < CORNER_LED_COUNT; i++) {
        rgblight_sethsv_at(h, s, v, corner_leds[i]);
    }
}

// Initialize RGB layer animation
void rgb_layers_init(void) {
    rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);
    for (int i = 0; i < CORNER_LED_COUNT; i++) {
        corner_led_colors[i] = 0;  // White
    }
    apply_base_layer_leds();
    rgb_animation_active = true;
    srand(timer_read());
    base_anim_timer = timer_read();
}

// Change one random corner LED to a random color
void animate_base_layer(void) {
    uint8_t corner_idx = rand() % CORNER_LED_COUNT;
    uint8_t color_idx = rand() % BASE_ANIM_COLOR_COUNT;
    corner_led_colors[corner_idx] = color_idx;
    uint8_t h = base_anim_colors[color_idx][0];
    uint8_t s = base_anim_colors[color_idx][1];
    uint8_t v = base_anim_colors[color_idx][2];
    rgblight_sethsv_at(h, s, v, corner_leds[corner_idx]);
}

// Main function called on layer change
void apply_layer_lighting(layer_state_t state, uint8_t base_layer_index) {
    uint8_t new_layer = get_highest_layer(state);

    if (new_layer == base_layer_index) {
        if (!rgb_animation_active) {
            rgb_animation_active = true;
            srand(timer_read());
            rgblight_sethsv_range(HSV_BLACK, 0, RGBLIGHT_LED_COUNT);
            apply_base_layer_leds();
            base_anim_timer = timer_read();
        }
    } else {
        if (rgb_animation_active) {
            rgb_animation_active = false;
        }
        apply_static_layer_colors(new_layer);
    }

    current_rgb_layer = new_layer;
    last_synced_layer = new_layer;
}

void rgb_layers_scan(uint8_t base_layer_index) {
    if (rgb_animation_active && get_highest_layer(layer_state) == base_layer_index) {
        if (timer_elapsed(base_anim_timer) > BASE_ANIM_INTERVAL) {
            animate_base_layer();
            base_anim_timer = timer_read();
        }
    }
}

bool rgb_layers_task(uint8_t base_layer_index) {
    uint8_t current_layer = get_highest_layer(layer_state);

    if (current_layer != last_synced_layer) {
        uprintf("Layer changed: %d -> %d\n", last_synced_layer, current_layer);
        apply_layer_lighting(layer_state, base_layer_index);
        return true;
    }
    return false;
}

// Get current HSV for a corner LED (for RPC sync)
void get_corner_led_hsv(uint8_t corner_idx, uint8_t* h, uint8_t* s, uint8_t* v) {
    if (corner_idx >= CORNER_LED_COUNT) {
        *h = 0; *s = 0; *v = 0;
        return;
    }

    uint8_t current_layer = get_highest_layer(layer_state);

    if (rgb_animation_active && current_layer == 0) {
        // BASE layer with animation - use animated colors
        uint8_t color_idx = corner_led_colors[corner_idx];
        *h = base_anim_colors[color_idx][0];
        *s = base_anim_colors[color_idx][1];
        *v = base_anim_colors[color_idx][2];
    } else {
        // Static layer colors
        if (current_layer > 7) current_layer = 0;
        *h = layer_corner_colors[current_layer][0];
        *s = layer_corner_colors[current_layer][1];
        *v = layer_corner_colors[current_layer][2];
    }
}
