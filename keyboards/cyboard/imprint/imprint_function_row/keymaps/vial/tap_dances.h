//-------------------------------------------------------------------------
// Tap Dances
//
// https://docs.qmk.fm/tap_hold
//-------------------------------------------------------------------------
enum {
    TD_0,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_0] = ACTION_TAP_DANCE_DOUBLE(KC_F12, LSFT(KC_F12))
};

int LT_GENERAL = 175;

//-------------------------------------------------------------------------
// Tapping Terms
//-------------------------------------------------------------------------

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        //-------------------------------------------------------------------------
        // Homerow
        //-------------------------------------------------------------------------
        // Pinky
        case LCTL_T(KC_A):
        case RCTL_T(KC_O):
            return 210;

        // Ring
        case LALT_T(KC_R):
        case RALT_T(KC_I):
            return 190;

        // Middle
        case LGUI_T(KC_S):
        case RGUI_T(KC_E):
            return 160;

        // Index
        case LSFT_T(KC_T):
        case RSFT_T(KC_N):
            return 135;

        //-------------------------------------------------------------------------
        // Others
        //-------------------------------------------------------------------------

        // case LSFT_T(KC_ESC):
        // case TD(TD_F12_SHIFT_F12):
        //     return HT_GENERAL;

        case LT(4, KC_G):
        case LT(1, KC_SPC):
            return LT_GENERAL;

        default:
            return TAPPING_TERM;
    }
}

//-------------------------------------------------------------------------
// Flow Tapping
//
// for MT and LT, if keys pressed within a short timeout of the preceeding key, the tapping behavior is triggered.
//-------------------------------------------------------------------------

// Filters on what is or is not a Flow Tap Key
bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false; // Disable Flow Tap on hotkeys.
    }

    switch (get_tap_keycode(keycode)) {
        //-------------------------------------------------------------------------
        // Homerow
        //-------------------------------------------------------------------------
        // Pinky
        case LCTL_T(KC_A):
        case RCTL_T(KC_O):
        // Ring
        case LALT_T(KC_R):
        case RALT_T(KC_I):
        // Middle
        case LGUI_T(KC_S):
        case RGUI_T(KC_E):
        // Index
        case LSFT_T(KC_T):
        case RSFT_T(KC_N):
            return true;

        //-------------------------------------------------------------------------
        // Others
        //-------------------------------------------------------------------------
        // default:
        //     // Fallback to the standard QMK logic (letters/space/punctuation)
        //     return is_flow_tap_key_default(keycode);
        // case KC_SPC:
        // case KC_A ... KC_Z:
        // case KC_DOT:
        // case KC_COMM:
        // case KC_SCLN:
        // case KC_SLSH:
        //     return true;
    }
    return false;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record,
                           uint16_t prev_keycode) {

    if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
        switch(keycode) {
            //-------------------------------------------------------------------------
            // Homerow
            //-------------------------------------------------------------------------
            // Index
            case LSFT_T(KC_T):
            case RSFT_T(KC_N):
                return 50;

            // Pinky
            case LCTL_T(KC_A):
            case RCTL_T(KC_O):
            // Ring
            case LALT_T(KC_R):
            case RALT_T(KC_I):
            // Middle
            case LGUI_T(KC_S):
            case RGUI_T(KC_E):
                return 100;

            case LT(4, KC_G):  // Layer Tap usually benefits from this too
                return 150;

            default:
                return FLOW_TAP_TERM;
        }
    }

    return FLOW_TAP_TERM;
}

//-------------------------------------------------------------------------
// Permissive Hold
//-------------------------------------------------------------------------

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // --- ENABLE FOR THUMB CLUSTERS ---
        // This allows you to tap-hold Space, hit 'A' quickly, and release Space
        // without waiting 160ms. It feels faster for layer navigation.
        case LT(1, KC_SPC):
            return true;

        // --- DISABLE FOR HOME ROW MODS ---
        // This forces QMK to ignore "rolling" inputs.
        // You must deliberately hold the 'A' key to get 'GUI'.
        case MT(MOD_LGUI, KC_A):
        case MT(MOD_LALT, KC_S):
        case MT(MOD_LCTL, KC_D):
        case MT(MOD_LSFT, KC_F):
        case MT(MOD_RSFT, KC_J):
        case MT(MOD_RCTL, KC_K):
        case MT(MOD_RALT, KC_L):
        case MT(MOD_RGUI, KC_SCLN):
            return false;

        default:
            return true; // Default behavior
    }
}

//-------------------------------------------------------------------------
// Quick Tap Term
//-------------------------------------------------------------------------

// uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
//     switch (keycode) {
//         // Pinky
//         case LCTL_T(KC_A):
//         case RCTL_T(KC_O):
//             return 210;
//
//         // Ring
//         case LALT_T(KC_R):
//         case RALT_T(KC_I):
//             return 250;
//
//         // Middle
//         case LGUI_T(KC_S):
//         case RGUI_T(KC_E):
//             return 250;
//
//         // Index
//         case LSFT_T(KC_T):
//         case RSFT_T(KC_N):
//             return 250;
//
//         default:
//             // Use the global setting from config.h
//             return QUICK_TAP_TERM;
//     }
// }

