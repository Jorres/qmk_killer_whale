ENCODER_MAP_ENABLE = no

KEY_OVERRIDE_ENABLE = yes # for overriding ctrl + h to backspace

CONSOLE_ENABLE = yes
COMBO_ENABLE = yes
WPM_ENABLE = yes

UNICODE_ENABLE = yes
UNICODE_COMMON = yes

# Include all .c files in the keymap directory (except keymap.c which is included automatically)
SRC += $(filter-out keymap.c,$(notdir $(wildcard $(KEYMAP_PATH)/*.c)))

# Include animation system files
SRC += animations/common.c animations/underglow.c animations/sequential.c
