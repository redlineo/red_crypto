#include "kc_to_ascii.h"

void crypto_process_record_user(uint16_t keycode, keyrecord_t *record, uint8_t readed_key) {
        switch (keycode) {
            case KC_1:
                if (record->event.pressed) {
                    readed_key[count_char_key] = ASCII_LOW_1;
                    count_char_key++;
                }
                break;
            default:
                break;
}
