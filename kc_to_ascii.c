// Author: redlineo, romanov.pd@gmail.com

#include "kc_to_ascii.h"

uint8_t shift_tracker  = 0;

uint8_t kc_to_ascii(uint16_t keycode, keyrecord_t *record, uint8_t readed_key[]) {
    switch (keycode) {
        case KC_LEFT_SHIFT:
            if (record->event.pressed) {
                register_code(KC_LEFT_SHIFT);
                shift_tracker = 1;
            }
            return 1;
            break;
        case KC_1:
            if (record->event.pressed) {
                if (shift_tracker == 1) {
                    unregister_code(KC_LEFT_SHIFT);
                    shift_tracker              = 0;
                    readed_key[count_char_key] = ASCII_UP_1;
                } else {
                    readed_key[count_char_key] = ASCII_LOW_1;
                }
                dprintf("%d",readed_key[count_char_key]);
                count_char_key++;
            }
            return 1;
            break;
        default:
            break;
    }
    return 0;
}
