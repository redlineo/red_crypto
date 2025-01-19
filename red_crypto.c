#include "red_crypto.h"



uint8_t dec_pass[2][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

uint8_t readed_key[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t count_char_key = 0;

uint8_t crypto_mode = 0;

void print_chars_w128(w128_t *x) {
    for (uint8_t i = 0; i < 16; i++) {
        if (x->b[i] == 0x00) break;
        printf("%c", x->b[i]);
    }
    dprintf("\n");
};

void print_chars(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        if (c[i] == 0x00) break;
        printf("%c", c[i]);
    }
    dprintf("\n");
};

void decrypt_pass_kuzn(const uint8_t encrypted_passwords[2][16]) {
    kuz_key_t key;
    w128_t    x;

    kuz_set_decrypt_key(&key, readed_key);
    for (uint8_t j = 0; j < 2; j++) {
        for (uint8_t i = 0; i < 16; i++) {
            x.b[i] = encrypted_passwords[j][i];
        }
        kuz_decrypt_block(&key, &x);

        dprintf("decrypted\t=");
        print_w128(&x);
        print_chars_w128(&x);

        for (uint8_t i = 0; i < 16; i++) {
            dec_pass[j][i] = x.b[i];
        }
    }

    // kuz_set_encrypt_key(&key,readed_key);
    // for (uint8_t i=0; i<16; i++) {
    //     x.b[i] = pass[i];
    // }
    // kuz_encrypt_block(&key,&x);
    // printf("encrypted\t="); print_w128(&x);
    // print_chars_w128(&x);
};

void send_chars_pass(uint8_t *out) {
    for (uint8_t i = 0; i < 16 && out[i] != 0; i++) {
        send_char((char)out[i]);
    }
};

void crypto_process_record_user(uint16_t keycode, keyrecord_t *record, const uint8_t encrypted_passwords[2][16]) {
    if (crypto_mode) {
        switch (keycode) {
            // TODO: write all ASCII printable keys
            // may be not printable, because we can use any byte of ASCII with QMK, HA-HA
            // transcode_keycode_to_ascii();
            // case KC_1:
            //     if (record->event.pressed) {
            //         readed_key[count_char_key] = ASCII_LOW_1;
            //         count_char_key++;
            //     }
            //     break;
            case GET_PASS:
                if (record->event.pressed) {
                    crypto_mode    = 0;
                    count_char_key = 0;
                    decrypt_pass_kuzn(encrypted_passwords);
                }
            default:
                kc_to_ascii(keycode, record, &readed_key);
                break;
        }
    } else {
        switch (keycode) {
            case RED_PASS1:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[0]);
                    // SEND_STRING(dec_pass[0]);
                }
                break;
            case RED_PASS2:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[1]);
                    // SEND_STRING(dec_pass[1]);
                }
                break;
            case RED_RNG:
                if (record->event.pressed) {
                    //TODO: change seed
                    for (uint8_t i = 0; i < 32; i++){
                        tap_random_base64();
                    }
                }
                break;
            case RED_TEST:
                #ifdef TEST_FUNCTIONS_ENABLED
                if (record->event.pressed) {
                    exec_sha256_test();
                    exec_kuznechik_test();
                    exec_aes_test();
                }
                #endif
                break;
            case GET_PASS:
                if (record->event.pressed) {
                    crypto_mode = 1;
                    for (uint8_t i = 0; i < 32; i++) {
                        readed_key[i] = 0;
                    }
                    dprintf("%d", keycode);
                }
                break;
        }
    }
}
