/**
 * Experimental
 *
 * Common functions to make simple tap dances.
**/

// =============================================================================
// UNIVERSAL TAP DANCE ENGINE
// =============================================================================

// 1. Define the Structure for your custom keys
typedef struct {
    uint16_t tap_kc;      // Key to send on Tap (e.g., KC_F12)
    uint16_t hold_kc;     // Key to send on Hold (e.g., S(KC_F12))
    uint16_t term;        // Custom Tapping Term (0 = use global default)
    bool     flow_tap;    // Enable Flow Tap for this specific key?
} custom_td_t;

// 2. Define your Enums (Add new names here)
enum {
    TD_SPACE_L1,   // Example: Space / Layer 1
    TD_F12_SHIFT,  // Example: F12 / Shift+F12
    TD_TAB_CTAB,   // Example: Tab / Ctrl+Tab
    // Add more here...
};

// 3. THE CONFIGURATION TABLE (EDIT THIS PART!)
// Order must match the Enum above.
static const custom_td_t custom_td_list[] = {
    // [ENUM_ID]      = { Tap Key,      Hold Key,      Term,  Flow Tap? }
    [TD_SPACE_L1]     = { KC_SPC,       MO(1),         160,   false },
    [TD_F12_SHIFT]    = { KC_F12,       S(KC_F12),     210,   false },
    [TD_TAB_CTAB]     = { KC_TAB,       C(KC_TAB),     200,   true  },
};

// =============================================================================
// LOGIC (DO NOT EDIT BELOW THIS LINE)
// =============================================================================

typedef struct {
    bool is_press_action;
    int state;
} tap_state_t;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
};

static tap_state_t td_state_global;

int cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else return SINGLE_TAP;
}

// Universal Finished (Key Down)
void common_td_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state_global.state = cur_dance(state);

    // Get the index based on which TD was triggered
    // Note: We use a hacky way to map 'user_data' if needed, but for simplicity
    // we assume strict ordering in the action array below.
    // However, standard QMK doesn't pass the index easily to this function without
    // defining a unique function for each.

    // To make this TRULY generic without 50 functions, we use the `user_data` field
    // in the action definition to pass the index.

    uint8_t index = (uint8_t)(uintptr_t)user_data;
    custom_td_t config = custom_td_list[index];

    switch (td_state_global.state) {
        case SINGLE_TAP:
            register_code16(config.tap_kc);
            break;
        case SINGLE_HOLD:
            // Check if it's a Layer (MO, LT) or a Key
            if ((config.hold_kc & 0xF000) == 0x5000) { // Check if MO/Layer
                 layer_on(config.hold_kc & 0xFF);
            } else {
                 register_code16(config.hold_kc);
            }
            break;
    }
}

// Universal Reset (Key Up)
void common_td_reset(qk_tap_dance_state_t *state, void *user_data) {
    uint8_t index = (uint8_t)(uintptr_t)user_data;
    custom_td_t config = custom_td_list[index];

    switch (td_state_global.state) {
        case SINGLE_TAP:
            unregister_code16(config.tap_kc);
            break;
        case SINGLE_HOLD:
             if ((config.hold_kc & 0xF000) == 0x5000) {
                 layer_off(config.hold_kc & 0xFF);
            } else {
                 unregister_code16(config.hold_kc);
            }
            break;
    }
    td_state_global.state = 0;
}

// Macro to generate the Actions automatically
#define TD_COMMON(index) ACTION_TAP_DANCE_FN_ADVANCED_USER(NULL, common_td_finished, common_td_reset, (void*)(uintptr_t)index)

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_SPACE_L1]  = TD_COMMON(TD_SPACE_L1),
    [TD_F12_SHIFT] = TD_COMMON(TD_F12_SHIFT),
    [TD_TAB_CTAB]  = TD_COMMON(TD_TAB_CTAB),
};

// keymap.c

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // 1. Check if it is one of our Custom Tap Dances
    // We iterate through the list to match the keycode
    for (int i = 0; i < sizeof(tap_dance_actions)/sizeof(tap_dance_actions[0]); i++) {
        if (keycode == TD(i)) {
            // Found it! Return custom term if set, otherwise global default
            return (custom_td_list[i].term > 0) ? custom_td_list[i].term : TAPPING_TERM;
        }
    }

    // 2. Handle other standard keys (Home Row Mods)
    switch (keycode) {
        case LSFT_T(KC_T): return 240;
        // ... your other manual overrides ...
        default: return TAPPING_TERM;
    }
}

bool get_flow_tap_term(uint16_t keycode, keyrecord_t *record) {
    // 1. Check our Custom Tap Dances
    for (int i = 0; i < sizeof(tap_dance_actions)/sizeof(tap_dance_actions[0]); i++) {
        if (keycode == TD(i)) {
            // Return TRUE if flow tap is enabled in the struct
            if (custom_td_list[i].flow_tap) return true; // (Or return specific time)
            else return false;
        }
    }

    // 2. Standard Logic
    switch(keycode) {
        // ... your other flow tap keys ...
        default: return false;
    }
}
