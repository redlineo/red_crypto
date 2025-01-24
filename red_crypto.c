// Author: redlineo, romanov.pd@gmail.com

#include "red_crypto.h"

void crypto_eeprom_init() {
    uint8_t storage_size     = INIT_STORAGE_SIZE;
    uint8_t storage_pass_len = INIT_STORAGE_PASS_LEN;
    typedef union {
        uint8_t raw[3 + 3 + 1 + 1 + 4 + 1 + (uint32_t)storage_size * (uint32_t)storage_pass_len];
        struct {
            uint8_t  init_var[3]; // if in eeprom is bytes "RED", then it's not first flash or first power-on, or in eeprom is old red_crypto
            uint8_t  version[3];  // red_crypto library version
            uint8_t  storage_size;
            uint8_t  storage_pass_len;
            uint32_t memory_usage;
            uint8_t  password_count;
            uint8_t  passwords[(uint32_t)storage_size * (uint32_t)storage_pass_len];
        };
    } red_crypto_storage;

    red_crypto_storage enc_pass;
    //TODO: don't work with this var,copy it to global var in RAM
    eeconfig_read_user_datablock(enc_pass.raw);
}

uint8_t *decrypted_passwords;

// initialize array "decrypted_passwords" with 0x00
void init_dec_pass(void) {
    // array = malloc(sizeof(int[ROWS][COLS])); // explicit 2D array notation
    decrypted_passwords = malloc(sizeof(uint8_t[enc_pass.storage_size][enc_pass.storage_pass_len]));
    for (uint8_t i = 0; i < enc_pass.storage_size; i++) {
        for (uint8_t j = 0; j < enc_pass.storage_pass_len; j++) {
            decrypted_passwords[i][j] = 0x00;
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

uint8_t decrypted_mode = 0;

uint8_t red_menu_mode = 0;

void encrypt_pass_kuzn() {
    kuz_key_t key;
    w128_t    x;

    init_dec_pass();

// for hashing key, like a KDF algorithm
#ifdef USE_SHA256_KEY
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, readed_key, min_len(u_strlen(readed_key), MAX_KEY_LEN));
    BYTE result_key[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, result_key);
    print_hex(result_key);
    kuz_set_encrypt_key(&key, result_key);
#endif
// if you don't have enough FLASH in MCU
#ifndef USE_SHA256_KEY
    kuz_set_encrypt_key(&key, readed_key);
#endif
}

// decrypting passwords
void decrypt_pass_kuzn() {
    kuz_key_t key;
    w128_t    x;

    init_dec_pass();

// for hashing key, like a KDF algorithm
#ifdef USE_SHA256_KEY
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, readed_key, min_len(u_strlen(readed_key), MAX_KEY_LEN));
    BYTE result_key[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, result_key);
    print_hex(result_key);
    kuz_set_decrypt_key(&key, result_key);
#endif
// if you don't have enough FLASH in MCU
#ifndef USE_SHA256_KEY
    kuz_set_decrypt_key(&key, readed_key);
#endif
    // decrypting all passwords in storage
    for (uint8_t password_index = 0; password_index < enc_pass.storage_size; password_index++) {
#ifdef USE_RED_CRY_DEBUG
        dprintf("decrypted\t=");
#endif
        for (uint8_t block_index = 0; block_index < enc_pass.storage_pass_len / 16; block_index++) {
            uint8_t cnt_zeros = 0;
            // checking is this empty block or not
            for (uint8_t byte_index = 0 + block_index * 16; byte_index < 16 + block_index * 16; byte_index++) {
                // if (encrypted_passwords[password_index][byte_index] == 0x00) {
                if (enc_pass.passwords[password_index * enc_pass.storage_pass_len + byte_index] == 0x00) {
                    cnt_zeros++;
                } else {
                    cnt_zeros = 0;
                    break;
                }
            }
            if (cnt_zeros != 0) break; // if there is zero 8 byte block in password, e.g. password length <32 or <64

            // reading password's block for decrypting
            for (uint8_t byte_index = 0 + block_index * 16; byte_index < 16 + block_index * 16; byte_index++) {
                // x.b[byte_index % 16] = encrypted_passwords[password_index][byte_index];
                x.b[byte_index % 16] = enc_pass.passwords[password_index * enc_pass.storage_pass_len + byte_index]
            }

            // decrypting via Kuznechik
            kuz_decrypt_block(&key, &x);

            // writing decrypted password's block
            for (uint8_t byte_index = 0 + block_index * 16; byte_index < 16 + block_index * 16; byte_index++) {
                decrypted_passwords[password_index][byte_index] = x.b[byte_index % 16];
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

// function for embedding to "proccess_record_user" for adding crypto features
uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (crypto_mode) {
        switch (keycode) {
            case RED_CRY_M:
                if (record->event.pressed) {
#ifdef USE_RED_CRY_DEBUG
                    print_int(readed_key);
#endif
                    crypto_mode    = 0;
                    count_char_key = 0;
                    decrypt_pass_kuzn();
                    decrypted_mode = 1;
                }
            default:
                // reading key
                return kc_to_ascii(keycode, record, readed_key, &count_char_key);
                break;
        }
    } else if (decrypted_mode && red_menu_mode) {
        uint8_t key_return = draw_red_menu(keycode, record);
        if (key_return == 2) {
            red_menu_mode = 0;
        } else {
            return key_return;
        }
    } else if (decrypted_mode) {
        switch (keycode) {
            case RED_PASS1:
                if (record->event.pressed) {
                    send_chars_pass(decrypted_passwords[0]);
                }
                break;
            case RED_PASS2:
                if (record->event.pressed) {
                    send_chars_pass(decrypted_passwords[1]);
                }
                break;
            case RED_PASS3:
                if (record->event.pressed) {
                    send_chars_pass(decrypted_passwords[2]);
                }
                break;
            case RED_PASS4:
                if (record->event.pressed) {
                    send_chars_pass(decrypted_passwords[3]);
                }
                break;
                // if you want to add more keys for passwords, copy this `case`
                // case RED_PASSX: //change X here
                //     if (record->event.pressed) {
                //         send_chars_pass(decrypted_passwords[X]); //change X here
                //     }
                //     break;
        }
    } else { // by default
        switch (keycode) {
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
                    crypto_mode = 1; // to enter in crypto mode block code above
                    // clearing readed key
                    for (uint8_t i = 0; i < 32; i++) {
                        readed_key[i] = 0;
                    }
                    // dprintf("%d", keycode);
                }
                break;
            case RED_MENU:
                if (record->event.pressed) {
                    if (decrypted_mode == 1) {
                        red_menu_mode = 1; // to enter in menu mode block code above
                    }
                }
                break;
        }
    }
    return 1; // for capturing entering of all keys from red_crypto
}
