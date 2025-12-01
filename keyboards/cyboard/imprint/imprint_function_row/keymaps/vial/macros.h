#include QMK_KEYBOARD_H

// 1. Define the structure of your data
//    This allows you to associate multiple behaviors with a single macro entry.
typedef struct {
    char *text_to_type; // The string to send
    bool press_enter;   // Whether to press Enter after typing
} macro_def_t;

// 2. Define your Custom Keycodes
//    We start at SAFE_RANGE to avoid conflicting with standard QMK codes.
enum custom_keycodes {
    MY_MACRO_START = SAFE_RANGE,

    // Define names for your macros here
    KC_EMAIL,
    KC_ADDR,
    KC_GITC,
    KC_ZOOM,

    MY_MACRO_END // Used to calculate range
};

// 3. The Data Array
//    IMPORTANT: The order here must match the order in the enum above!
//    KC_EMAIL maps to index 0, KC_ADDR to index 1, etc.
static const macro_def_t macro_definitions[] = {
    // { "Text to type",  Press Enter? }

    [KC_EMAIL - MY_MACRO_START] = { "user@example.com", false },
    [KC_ADDR]                   = { "123 Keyboard Ave, Switch City", true },
    [KC_GITC]                   = { "git commit -m \"update\"", true },
    [KC_ZOOM]                   = { "https://zoom.us/j/123456789", true },
};

// 4. The Logic Processor
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If the key is pressed (not released)
    if (record->event.pressed) {

        // Check if the keycode is within our macro range
        if (keycode >= MY_MACRO_START && keycode < MY_MACRO_END) {

            // Calculate the array index
            uint8_t index = keycode - MY_MACRO_START;

            // Safety check: Ensure we don't read outside the array
            if (index < (sizeof(macro_definitions) / sizeof(macro_def_t))) {

                // Get the data struct
                macro_def_t macro = macro_definitions[index];

                // execute the logic
                send_string(macro.text_to_type);

                if (macro.press_enter) {
                    tap_code(KC_ENT);
                }
            }

            return false; // Tell QMK we handled this key, don't do anything else
        }
    }
    return true; // Process all other keys normally
}

// 5. The Keymap
//    Assign the custom keycodes (KC_EMAIL, etc.) to your layers.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_EMAIL, KC_ADDR, KC_GITC, KC_ZOOM
        // ... rest of your keyboard layout ...
    )
};
