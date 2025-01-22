// Author: redlineo, romanov.pd@gmail.com

#include "red_crypto.h"

uint8_t dec_pass[STORAGE_SIZE][STORAGE_PASS_LEN];

void init_dec_pass() {
    for (uint8_t i = 0; i < STORAGE_SIZE; i++) {
        for (uint8_t j = 0; j < STORAGE_PASS_LEN; j++) {
            dec_pass[i][j] = 0x00;
        }
    }
}

// you can use key any length with SHA256, kuznechik use only 32byte
#ifdef USE_SHA256_KEY
uint8_t readed_key[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
#ifndef USE_SHA256_KEY
uint8_t readed_key[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

uint8_t count_char_key = 0;

uint8_t crypto_mode = 0;

void print_chars_w128(w128_t *x) {
    for (uint8_t i = 0; i < 16; i++) {
        if (x->b[i] == 0x00) break;
        dprintf("%c", x->b[i]);
    }
    dprintf("\n");
};

void print_chars(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        if (c[i] == 0x00) break;
        dprintf("%c", c[i]);
    }
    dprintf("\n");
};

void print_int(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        dprintf("%d", c[i]);
    }
    dprintf("\n");
};

void print_hex(uint8_t *h) {
    for (uint8_t i = 0; i < 32; i++) {
        dprintf("%x ", h[i]);
    }
    dprintf("\n");
};

size_t min_len(size_t a, size_t b) {
    if (a < b)
        return a;
    else
        return b;
}

void decrypt_pass_kuzn(const uint8_t encrypted_passwords[STORAGE_SIZE][STORAGE_PASS_LEN]) {
    kuz_key_t key;
    w128_t    x;

    init_dec_pass();

#ifdef USE_SHA256_KEY
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, readed_key, min_len(u_strlen(readed_key), MAX_KEY_LEN));
    BYTE result_key[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, result_key);
    print_hex(result_key);
    kuz_set_decrypt_key(&key, result_key);
#endif
#ifndef USE_SHA256_KEY
    kuz_set_decrypt_key(&key, readed_key);
#endif
    for (uint8_t j = 0; j < STORAGE_SIZE; j++) {
#ifdef USE_RED_CRY_DEBUG
        dprintf("decrypted\t=");
#endif
        for (uint8_t k = 0; k < STORAGE_PASS_LEN / 16; k++) {
            uint8_t cnt_zeros = 0;
            for (uint8_t z = 0 + k * 16; z < 16 + k * 16; z++) {
                if (encrypted_passwords[j][z] == 0x00) {
                    cnt_zeros++;
                } else {
                    cnt_zeros = 0;
                    break;
                }
            }
            if (cnt_zeros != 0) break; // if there is zero 8 byte block in password, e.g. password length <32 or <64

            for (uint8_t i = 0 + k * 16; i < 16 + k * 16; i++) {
                x.b[i % 16] = encrypted_passwords[j][i];
            }

            kuz_decrypt_block(&key, &x);

            for (uint8_t i = 0 + k * 16; i < 16 + k * 16; i++) {
                dec_pass[j][i] = x.b[i % 16];
            }

#ifdef USE_RED_CRY_DEBUG
            print_w128(&x);
            print_chars_w128(&x);
#endif
        }
    }
};

// TODO: add encrypt new passwords with binding on new keys
// and storing all these to EEPROM
// void encrypt_pass_kuzn(){
// kuz_set_encrypt_key(&key,readed_key);
// for (uint8_t i=0; i<16; i++) {
//     x.b[i] = pass[i];
// }
// kuz_encrypt_block(&key,&x);
// printf("encrypted\t="); print_w128(&x);
// print_chars_w128(&x);
// }

void send_chars_pass(uint8_t *out) {
    for (uint8_t i = 0; i < STORAGE_PASS_LEN && out[i] != 0; i++) {
        send_char((char)out[i]);
    }
};

uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record, const uint8_t encrypted_passwords[STORAGE_SIZE][STORAGE_PASS_LEN]) {
    if (crypto_mode) {
        switch (keycode) {
            case RED_CRY_M:
                if (record->event.pressed) {
#ifdef USE_RED_CRY_DEBUG
                    print_int(readed_key);
#endif
                    crypto_mode    = 0;
                    count_char_key = 0;
                    decrypt_pass_kuzn(encrypted_passwords);
                }
            default:
                // reading key
                return kc_to_ascii(keycode, record, readed_key, &count_char_key);
                break;
        }
    } else {
        switch (keycode) {
            case RED_PASS1:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[0]);
                }
                break;
            case RED_PASS2:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[1]);
                }
                break;
            case RED_PASS3:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[2]);
                }
                break;
            case RED_PASS4:
                if (record->event.pressed) {
                    send_chars_pass(dec_pass[3]);
                }
                break;
            // if you want to add more keys for passwords, copy this `case`
            // case RED_PASSX: //change X here
            //     if (record->event.pressed) {
            //         send_chars_pass(dec_pass[X]); //change X here
            //     }
            //     break;
            case RED_RNG:
                if (record->event.pressed) {
                    // TODO: change seed
                    for (uint8_t i = 0; i < 32; i++) {
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
            case RED_CRY_M:
                if (record->event.pressed) {
                    crypto_mode = 1;
                    for (uint8_t i = 0; i < 32; i++) {
                        readed_key[i] = 0;
                    }
                    // dprintf("%d", keycode);
                }
                break;
        }
    }
    return 0;
}
