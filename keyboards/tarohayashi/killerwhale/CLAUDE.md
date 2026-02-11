# Killer Whale Keyboard - Codebase Guide

## First and foremost advice to Claude

When finishing a new feature or a refactoring, YOU MUST check if you need to modify or cleanup this guide for future versions of yourself. Core concepts will likely stay the same, but file names, filetree structure and APIs might absolutely change.

## Overview
This is a QMK firmware for the Killer Whale split keyboard (Tarohayashi design). The keyboard has two halves connected via TRRS, with 66 RGB LEDs total (33 per half, indexed 0-32 left, 33-65 right).

**Key Detail**: The master half is determined by which side has the USB cable plugged in, not by physical position. Use `is_keyboard_left()` for physical position checks.

## Directory Structure

```
duo/keymaps/jorres/
├── keymap.c                    # Main keymap, layer definitions, core logic
├── custom_keycodes.h           # Custom keycode enum definitions
├── process_record.c/h          # Custom keycode implementations
├── lang_keys.c/h               # Language-aware keys (table-driven, see below)
├── rgb_layers.c/h              # RGB layer system (corner LEDs, shared color palette)
├── led_map.c/h                 # Matrix position to LED index mapping
├── animations/
│   ├── interface.h             # Shared types, animation mode enum, RPC definitions
│   ├── common.c                # Animation dispatcher and LED utilities
│   ├── underglow.c/h           # Corner LED animation + reactive key flash
│   └── sequential.c/h          # Sequential LED stepping on key press
├── host/                       # Host-side Go daemon for Raw HID layout sync
│   ├── main.go
│   └── go.mod
└── rules.mk                    # Build configuration
```

## LED Mapping System

### Matrix-to-LED Mapping (`led_map.c/h`)
Defines which LED corresponds to each physical key position:
- `matrix_to_led[row][col]` returns LED index (0-65) or 255 if no LED
- **Independent of layers** - maps physical switches to LEDs
- Used by reactive key flash feature

Example usage:
```c
#include "led_map.h"
uint8_t led_index = pgm_read_byte(&matrix_to_led[row][col]);
```

## Color Palette System

### Shared Color Palette (`rgb_layers.c/h`)
```c
const uint8_t underglow_color_palette[][3] = {
    {HSV_WHITE}, {HSV_CYAN}, {HSV_BLACK}, {HSV_ORANGE}, {HSV_BLACK}
};
```
- Shared between underglow animation and key flash
- Black entries (V=0) used by underglow for animation pauses
- Key flash skips black entries automatically

## Animation System Architecture

### Core Concept
The animation system uses a **dispatcher pattern** with mode-based routing. All animations implement two callbacks:
- `*_on_housekeeping()` - Called every housekeeping cycle (~1ms)
- `*_on_keypress(row, col)` - Called when any key is pressed

### Animation Modes
Defined in `animations/interface.h`:
```c
typedef enum {
    ANIMATION_UNDERGLOW,    // Default: Corner LED animation + reactive key flash
    ANIMATION_SEQUENTIAL    // Sequential LED stepping
} animation_mode_t;
```

### Key Files

**`animations/interface.h`**
- RPC transaction types (`colors_to_slave_t`, `layer_to_slave_t`)
- Animation mode enum
- Interface function declarations

**`animations/common.c`**
- `update_leds_on_housekeeping(mode)` - Routes to animation housekeeping
- `update_leds_on_keypress(mode, row, col)` - Routes keypress with position
- `clear_all_leds()` - Clears all 66 LEDs on both halves

**`animations/underglow.c`**
- **Corner LED animation**: Timer-based animation of 8 corner LEDs
- **Reactive key flash**: Lights up pressed keys for 500ms
  - Random color from shared palette (excluding black)
  - Reduced brightness (115/255)
  - Tracks up to 10 simultaneous flashes
- Layer state sync to slave via `SLAVE_LAYER_REFRESH` RPC
- Uses shared `underglow_color_palette` from `rgb_layers.h`
- Uses `led_map` for key-to-LED mapping

**`animations/sequential.c`**
- Key-press triggered sequential LED stepping (0→65)
- Uses `is_led_on_master()` helper to determine master vs slave LEDs
- Only runs on key press, no-op on housekeeping

### RPC Communication
Split keyboard halves communicate via RPC transactions:
- `RPC_ANIMATION_STEP` - Send LED color data to slave (reused for multiple purposes)
- `SLAVE_LAYER_REFRESH` - Notify slave of layer changes

Transaction IDs defined in `duo/config.h`:
```c
#define SPLIT_TRANSACTION_IDS_USER RPC_ANIMATION_STEP, SLAVE_LAYER_REFRESH
```

**Important**: Multiple systems can use the same RPC transaction ID with separate invocations. The underglow system sends separate RPC calls for corner animation and key flash, both using `RPC_ANIMATION_STEP`.

## Master/Slave LED Mapping

**Critical**: LED indices are fixed (0-32 left, 33-65 right), but which half is master depends on USB cable position.

```c
// Helper to determine if LED is on master side
static bool is_led_on_master(uint8_t led_index) {
    if (is_keyboard_left()) {
        return led_index <= 32;  // Left master: LEDs 0-32
    } else {
        return led_index >= 33;  // Right master: LEDs 33-65
    }
}
```

**Master LEDs**: Set directly via `rgblight_sethsv_at(h, s, v, index)`
**Slave LEDs**: Send via `transaction_rpc_send(RPC_ANIMATION_STEP, ...)`

## Key Integration Points

### `keymap.c`

**Critical ordering in `process_record_user()`**:
```c
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // 1. Trigger animations FIRST (before custom keycodes)
    //    This ensures all physical keypresses trigger animations,
    //    even if custom keycodes return false
    if (record->event.pressed && keycode != ANIM_CYCLE) {
        update_leds_on_keypress(current_animation, record->event.key.row, record->event.key.col);
    }

    // 2. Handle ANIM_CYCLE mode switching
    // 3. Process custom keycodes
    // 4. Process Luna/OLED updates
}
```

**Why this order matters**: Custom keycodes (like PRTSCR) return `false`, which would exit early and prevent animations from running. By calling animations first, all keypresses are captured.

**`matrix_scan_user()`**:
- Runs `rgb_layers_scan(0)` for underglow mode
- This drives the corner LED timer-based animation

**`housekeeping_task_user()`**:
- Routes to `update_leds_on_housekeeping(current_animation)`
- Underglow uses this for key flash timeouts and corner LED animation sync

## Adding a New Animation

1. **Create files**: `animations/newmode.c` and `animations/newmode.h`
2. **Implement callbacks**:
   ```c
   void newmode_on_housekeeping(void) { /* your logic */ }
   void newmode_on_keypress(uint8_t row, uint8_t col) { /* your logic */ }
   ```
3. **Add to enum** in `interface.h`:
   ```c
   typedef enum {
       ANIMATION_UNDERGLOW,
       ANIMATION_SEQUENTIAL,
       ANIMATION_NEWMODE  // Add here
   } animation_mode_t;
   ```
4. **Add to dispatcher** in `common.c`:
   ```c
   case ANIMATION_NEWMODE:
       newmode_on_housekeeping(); // or _on_keypress(row, col)
       break;
   ```
5. **Update `rules.mk`**: Add `animations/newmode.c` to `SRC +=` (or rely on wildcard)
6. **Update cycling logic** in `keymap.c`: Change `% 2` to `% 3` (or number of modes)

## Language-Aware Keys (`lang_keys.c/h`)

Keys that produce the correct character regardless of the active OS keyboard layout (US vs Russian). A host-side daemon pushes layout changes to the keyboard via Raw HID.

### Architecture
```
GNOME layout change → Go daemon (host/main.go) → Raw HID → keymap.c raw_hid_receive() → current_os_layout global
```

### How it works
- `current_os_layout` (in `keymap.c`) is set by the host daemon: 0=US, 1=RU
- `lang_keys.c` uses a **table-driven** approach — each key maps to a `{us_keycode, ru_keycode}` pair
- `process_record.c` delegates to `process_lang_key()` for keycodes in `[LANG_KEY_START, LANG_KEY_END]`

### Adding a new language-aware key
1. Add the keycode to `custom_keycodes.h` between `LANG_KEY_START` and `LANG_KEY_END`, update `LANG_KEY_END`:
   ```c
   LANG_KEY_START,
   QUES_LANG = LANG_KEY_START,
   NEWKEY_LANG,
   LANG_KEY_END = NEWKEY_LANG,
   ```
2. Add the mapping in `lang_keys.c`:
   ```c
   [NEWKEY_LANG - LANG_KEY_START] = { S(KC_X), KC_Y },
   ```
3. Place the keycode in the desired layer in `keymap.c`

### Raw HID Protocol
- Packet: 32 bytes, byte[0] = command, byte[1] = payload
- Command `0x01`: layout update, byte[1] = layout index (0=US, 1=RU)

## Common Pitfalls

1. **LED indices**: Always use helpers to check master/slave, don't hardcode `< 33`
2. **RPC batching**: `colors_to_slave_t` holds max 8 LEDs, batch accordingly
3. **Keypress ordering**: Animation calls must happen BEFORE custom keycode processing
4. **Clearing LEDs**: Use `clear_all_leds()` when switching modes to prevent remnants
5. **Color palette**: Use shared `underglow_color_palette` from `rgb_layers.h`, don't duplicate

## Building
```bash
qmk compile -kb tarohayashi/killerwhale/duo -km jorres
```
