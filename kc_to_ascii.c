// Author: redlineo, romanov.pd@gmail.com

#include "kc_to_ascii.h"

uint8_t shift_tracker = 0;

void write_keycode(uint8_t up_key, uint8_t low_key, keyrecord_t *record, uint8_t readed_key[], uint8_t *count_char_key) {
    if (record->event.pressed) {
        if (shift_tracker == 1) {
            // shift_tracker               = 0;
            readed_key[*count_char_key] = up_key;
        } else {
            readed_key[*count_char_key] = low_key;
        }
#ifdef USE_RED_CRY_DEBUG
        dprintf("%d", readed_key[*count_char_key]);
#endif
        *count_char_key = *count_char_key + 1;
    }
};

uint8_t kc_to_ascii(uint16_t keycode, keyrecord_t *record, uint8_t readed_key[], uint8_t *count_char_key) {
    switch (keycode) {
        case KC_LEFT_SHIFT:
            if (record->event.pressed) {
                shift_tracker = 1;
            }
            else {
                shift_tracker = 0;
            }
            return 1;
            break;
        case KC_A:
            write_keycode(ASCII_UP_A, ASCII_LOW_A, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_B:
            write_keycode(ASCII_UP_B, ASCII_LOW_B, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_C:
            write_keycode(ASCII_UP_C, ASCII_LOW_C, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_D:
            write_keycode(ASCII_UP_D, ASCII_LOW_D, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_E:
            write_keycode(ASCII_UP_E, ASCII_LOW_E, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_F:
            write_keycode(ASCII_UP_F, ASCII_LOW_F, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_G:
            write_keycode(ASCII_UP_G, ASCII_LOW_G, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_H:
            write_keycode(ASCII_UP_H, ASCII_LOW_H, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_I:
            write_keycode(ASCII_UP_I, ASCII_LOW_I, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_J:
            write_keycode(ASCII_UP_J, ASCII_LOW_J, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_K:
            write_keycode(ASCII_UP_K, ASCII_LOW_K, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_L:
            write_keycode(ASCII_UP_L, ASCII_LOW_L, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_M:
            write_keycode(ASCII_UP_M, ASCII_LOW_M, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_N:
            write_keycode(ASCII_UP_N, ASCII_LOW_N, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_O:
            write_keycode(ASCII_UP_O, ASCII_LOW_O, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_P:
            write_keycode(ASCII_UP_P, ASCII_LOW_P, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_Q:
            write_keycode(ASCII_UP_Q, ASCII_LOW_Q, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_R:
            write_keycode(ASCII_UP_R, ASCII_LOW_R, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_S:
            write_keycode(ASCII_UP_S, ASCII_LOW_S, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_T:
            write_keycode(ASCII_UP_T, ASCII_LOW_T, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_U:
            write_keycode(ASCII_UP_U, ASCII_LOW_U, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_V:
            write_keycode(ASCII_UP_V, ASCII_LOW_V, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_W:
            write_keycode(ASCII_UP_W, ASCII_LOW_W, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_X:
            write_keycode(ASCII_UP_X, ASCII_LOW_X, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_Y:
            write_keycode(ASCII_UP_Y, ASCII_LOW_Y, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_Z:
            write_keycode(ASCII_UP_Z, ASCII_LOW_Z, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_1:
            write_keycode(ASCII_UP_1, ASCII_LOW_1, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_2:
            write_keycode(ASCII_UP_2, ASCII_LOW_2, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_3:
            write_keycode(ASCII_UP_3, ASCII_LOW_3, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_4:
            write_keycode(ASCII_UP_4, ASCII_LOW_4, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_5:
            write_keycode(ASCII_UP_5, ASCII_LOW_5, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_6:
            write_keycode(ASCII_UP_6, ASCII_LOW_6, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_7:
            write_keycode(ASCII_UP_7, ASCII_LOW_7, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_8:
            write_keycode(ASCII_UP_8, ASCII_LOW_8, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_9:
            write_keycode(ASCII_UP_9, ASCII_LOW_9, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_0:
            write_keycode(ASCII_UP_0, ASCII_LOW_0, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_SPACE:
            write_keycode(ASCII_SPACE, ASCII_SPACE, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_MINUS:
            write_keycode(ASCII_UP_MINUS, ASCII_LOW_MINUS, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_EQUAL:
            write_keycode(ASCII_UP_EQUAL, ASCII_LOW_EQUAL, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_LEFT_BRACKET:
            write_keycode(ASCII_UP_LEFT_BR, ASCII_LOW_LEFT_BR, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_RIGHT_BRACKET:
            write_keycode(ASCII_UP_RIGHT_BR, ASCII_LOW_RIGHT_BR, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_BACKSLASH:
            write_keycode(ASCII_UP_BACKSLASH, ASCII_LOW_BACKSLASH, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_NONUS_HASH:
            write_keycode(ASCII_UP_HASH, ASCII_GRAVE, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_SEMICOLON:
            write_keycode(ASCII_UP_SEMICOLON, ASCII_LOW_SEMICOLON, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_QUOTE:
            write_keycode(ASCII_UP_QUOTE, ASCII_LOW_QUOTE, record, readed_key, count_char_key);
            return 1;
            break;
        // i am not sure about KC_GRAVE and KC_NONUS_HASH
        // need to test
        case KC_GRAVE:
            write_keycode(ASCII_NON_PRINTABLE, ASCII_GRAVE, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_COMMA:
            write_keycode(ASCII_UP_COMMA, ASCII_LOW_COMMA, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_DOT:
            write_keycode(ASCII_UP_DOT, ASCII_LOW_DOT, record, readed_key, count_char_key);
            return 1;
            break;
        case KC_SLASH:
            write_keycode(ASCII_UP_SLASH, ASCII_LOW_SLASH, record, readed_key, count_char_key);
            return 1;
            break;
        default:
            break;
    }
    return 0;
}