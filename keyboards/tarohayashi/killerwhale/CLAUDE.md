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
├── rgb_layers.c/h              # RGB layer system (corner LEDs, colors)
├── animations/
│   ├── interface.h             # Shared types, animation mode enum, RPC definitions
│   ├── common.c                # Animation dispatcher and LED utilities
│   ├── underglow.c/h           # Timer-based corner LED animation
│   └── sequential.c/h          # Key-press triggered sequential LED animation
└── rules.mk                    # Build configuration
```

## Animation System Architecture

### Core Concept
The animation system uses a **dispatcher pattern** with mode-based routing. All animations implement two callbacks:
- `*_on_housekeeping()` - Called every housekeeping cycle (~1ms)
- `*_on_keypress()` - Called when any key is pressed

### Animation Modes
Defined in `animations/interface.h`:
```c
typedef enum {
    ANIMATION_UNDERGLOW,    // Default: Corner LED animation
    ANIMATION_SEQUENTIAL    // Sequential LED stepping
} animation_mode_t;
```

### Key Files

**`animations/interface.h`**
- RPC transaction types (`colors_to_slave_t`, `layer_to_slave_t`)
- Animation mode enum
- Function declarations

**`animations/common.c`**
- `update_leds_on_housekeeping(mode)` - Routes to appropriate animation
- `update_leds_on_keypress(mode)` - Routes to appropriate animation
- `clear_all_leds()` - Clears all 66 LEDs on both halves (handles master/slave correctly)

**`animations/underglow.c`**
- Timer-based animation updating 8 corner LEDs
- Syncs layer state to slave via `SLAVE_LAYER_REFRESH` RPC
- Only runs on housekeeping, no-op on key press

**`animations/sequential.c`**
- Key-press triggered sequential LED stepping (0→65)
- Uses `is_led_on_master()` helper to determine master vs slave LEDs
- Only runs on key press, no-op on housekeeping

### RPC Communication
Split keyboard halves communicate via RPC transactions:
- `RPC_ANIMATION_STEP` - Send LED color data to slave
- `SLAVE_LAYER_REFRESH` - Notify slave of layer changes

Transaction IDs defined in `duo/config.h`:
```c
#define SPLIT_TRANSACTION_IDS_USER RPC_ANIMATION_STEP, SLAVE_LAYER_REFRESH
```

## Master/Slave LED Mapping

**Critical**: LED indices are fixed (0-32 left, 33-65 right), but which half is master depends on USB cable position.

```c
// Determine if LED is on master side
if (is_keyboard_left()) {
    // Master is left: LEDs 0-32 are master, 33-65 are slave
} else {
    // Master is right: LEDs 33-65 are master, 0-32 are slave
}
```

Master LEDs: Set directly via `rgblight_sethsv_at(h, s, v, index)`
Slave LEDs: Send via `transaction_rpc_send(RPC_ANIMATION_STEP, ...)`

## Key Integration Points

### `keymap.c`

**Animation mode tracking**:
```c
static animation_mode_t current_animation = ANIMATION_UNDERGLOW;
```

**`matrix_scan_user()`**:
- Conditionally runs `rgb_layers_scan(0)` only for underglow mode
- Prevents interference with other animations

**`process_record_user()`**:
- Handles `ANIM_CYCLE` keycode to switch modes
- Calls `clear_all_leds()` before switching
- Routes key presses to `update_leds_on_keypress(current_animation)`

**`housekeeping_task_user()`**:
- Routes housekeeping to `update_leds_on_housekeeping(current_animation)`

## Adding a New Animation

1. **Create files**: `animations/newmode.c` and `animations/newmode.h`
2. **Implement callbacks**:
   ```c
   void newmode_on_housekeeping(void) { /* your logic */ }
   void newmode_on_keypress(void) { /* your logic */ }
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
       newmode_on_housekeeping(); // or _on_keypress()
       break;
   ```
5. **Update `rules.mk`**: Add `animations/newmode.c` to `SRC +=`
6. **Update cycling logic** in `keymap.c`: Change `% 2` to `% 3` (or number of modes)

## Common Pitfalls

1. **LED indices**: Always use helpers to check master/slave, don't hardcode `< 33`
2. **RPC batching**: `colors_to_slave_t` holds max 8 LEDs, batch accordingly
3. **Animation interference**: Ensure `rgb_layers_scan()` only runs when needed
4. **Clearing LEDs**: Use `clear_all_leds()` when switching modes to prevent remnants

## Building
```bash
qmk compile -kb tarohayashi/killerwhale/duo -km jorres
```

