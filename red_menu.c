#include "red_menu.h"

// clang-format off
enum menu_options {
    EXIT,
    SHOW_PASS_NAMES,
    SHOW_MEMORY_USAGE,
    CHANGE_MASTER_KEY,
    CHANGE_PASS_IN_STORAGE,
    CHANGE_PASS_NAME,
    ADD_NEW_PASS,
    SAVE_NEW_PASS,
    READING_PASS,
    READ_USER_CHOICE,
    SHOW_THIS_MENU
};

// clang-format on
uint8_t  readed_option[1] = {255};
uint8_t  count_readed     = 0;
uint8_t  return_key       = 0;
uint8_t *enc_pass_new;

uint8_t readed_key_new[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t count_char_key_new  = 0;

// read new key
uint8_t read_new_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_ENTER:
            readed_option[0] = SAVE_NEW_PASS;
            return 1;
            break;
        default:
            if (count_char_key_new + 1 == enc_pass.storage_pass_len) {
                readed_option[0] = SAVE_NEW_PASS;
                send_string("Length limit!\n");
                // TODO: done this feature
            }
            return kc_to_ascii(keycode, record, readed_key_new, &count_char_key_new);
            break;
    }
    return 0;
}

// clang-format on
// 0 idle
// 1 get key input
// 2 exit menu
uint8_t draw_red_menu(uint16_t keycode, keyrecord_t *record) {
    switch (readed_option[0]) {
        case EXIT:
            readed_option[0] = 255;
            return 2;
            break;
        case READ_USER_CHOICE: {
            return_key   = kc_to_ascii(keycode, record, readed_option, &count_readed);
            count_readed = 0;
            if (readed_option[0] == ASCII_LOW_0) {
                readed_option[0] = EXIT;
            }
            if (readed_option[0] == ASCII_LOW_1) {
                readed_option[0] = SHOW_PASS_NAMES;
            }
            if (readed_option[0] == ASCII_LOW_2) {
                readed_option[0] = SHOW_MEMORY_USAGE;
            }
            if (readed_option[0] == ASCII_LOW_3) {
                readed_option[0] = CHANGE_MASTER_KEY;
            }
            if (readed_option[0] == ASCII_LOW_4) {
                readed_option[0] = CHANGE_PASS_IN_STORAGE;
            }
            if (readed_option[0] == ASCII_LOW_5) {
                readed_option[0] = CHANGE_PASS_NAME;
            }
            if (readed_option[0] == ASCII_LOW_6) {
                readed_option[0] = ADD_NEW_PASS;
            }
            if (return_key == 1) {
                send_string(SS_DOWN(X_LCTL) SS_DOWN(X_A) SS_UP(X_LCTL) SS_UP(X_A) SS_TAP(X_BACKSPACE));
                return 1;
            }
            break;
        }
        case SHOW_PASS_NAMES:
        case SHOW_MEMORY_USAGE:
        case CHANGE_MASTER_KEY:
        case CHANGE_PASS_IN_STORAGE:
        case CHANGE_PASS_NAME:
        case SAVE_NEW_PASS:
            enc_pass_new = (uint8_t *)malloc(sizeof(uint8_t) * enc_pass.storage_pass_len);
            enc_pass_new = encrypt_pass_kuzn(readed_key_new, count_char_key_new);
            for (uint8_t new_pass_index = 0; new_pass_index < enc_pass.storage_pass_len; new_pass_index++) {
                enc_pass.passwords[enc_pass.password_count * enc_pass.storage_pass_len + new_pass_index] = enc_pass_new[new_pass_index];
            }
            enc_pass.password_count++;
            // eeconfig_update_user_datablock(enc_pass.raw);
            red_write_to_storage();
            send_string("Successful adding new key on keycode RED_PASS");
            send_byte(enc_pass.password_count);
            send_string("\n");
            send_string(SS_DELAY(1000));
            readed_option[0] = 255;
            break;
        case READING_PASS:
            return read_new_key(keycode, record);
            break;
        case ADD_NEW_PASS:
            if (enc_pass.password_count == enc_pass.storage_size) {
                send_string("Limit error\n");
                readed_option[0] = 255;
            } else {
                send_string("Write new password. Tap `Enter` for confirm\n");
                readed_option[0] = READING_PASS;
            }
            break;
        default: // SHOW_THIS_MENU
            send_string("RED_CRYPTO MENU:\n0-EXIT\n1-Show password's names\n2-Show memory usage\n3-Change master key\n4-Change password in storage\n5-Change password's name\n6-Add new password\n");
            send_string("Choose option(0-6): ");
            readed_option[0] = READ_USER_CHOICE;
            break;
    }
    return 0;
}