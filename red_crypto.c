// Author: redlineo, romanov.pd@gmail.com

#include "red_crypto.h"

uint8_t eeprom_inited = 0;

void crypto_eeprom_init() {
    uint8_t red_var[3] = {'R', 'E', 'D'};
    // TODO: don't work with this var,copy it to global var in RAM
    // TODO: write code for not emulated EEPROM
    // eeconfig_read_user_datablock(enc_pass.raw);
    // for (uint8_t i = 0; i < EECONFIG_USER_DATA_SIZE; i++) {
    //     dprintf("%x ", enc_pass.raw[i]);
    // }

    red_read_from_storage();

    if (enc_pass.init_var[0] == red_var[0] && enc_pass.init_var[1] == red_var[1] && enc_pass.init_var[2] == red_var[2] && enc_pass.password_count > 0) { // not empty eeprom
        dprintf("Not empty EEPROM\n");
        eeprom_inited = 1;
        // #undef INIT_STORAGE_SIZE
        // #define INIT_STORAGE_SIZE enc_pass.storage_size
        // #undef INIT_STORAGE_PASS_LEN
        // #define INIT_STORAGE_PASS_LEN enc_pass.storage_pass_len
    } else { // empty eeprom
        dprintf("Empty EEPROM\n");
        eeprom_inited             = 0;
        enc_pass.init_var[0]      = 'R';
        enc_pass.init_var[1]      = 'E';
        enc_pass.init_var[2]      = 'D';
        enc_pass.version[0]       = '0';
        enc_pass.version[1]       = '6';
        enc_pass.version[2]       = '5';
        enc_pass.memory_usage     = 0;
        enc_pass.password_count   = 0;
        enc_pass.storage_size     = INIT_STORAGE_SIZE;
        enc_pass.storage_pass_len = INIT_STORAGE_PASS_LEN;
        for (uint8_t password_index = 0; password_index < enc_pass.storage_size; password_index++) { // must zero bytes in memory
            for (uint8_t password_byte = 0; password_byte < enc_pass.storage_pass_len; password_byte++) {
                enc_pass.passwords[password_index * enc_pass.storage_pass_len + password_byte] = 0x00;
            }
        }
        // eeconfig_update_user_datablock(enc_pass.raw);
        red_write_to_storage();
    }
}

uint8_t **decrypted_passwords;

// initialize array "decrypted_passwords" with 0x00
void init_dec_pass(void) {
    decrypted_passwords = (uint8_t **)malloc(sizeof(uint8_t *) * enc_pass.storage_size);
    for (uint8_t i = 0; i < enc_pass.storage_size; i++) {
        decrypted_passwords[i] = (uint8_t *)malloc(sizeof(uint8_t) * enc_pass.storage_pass_len);
    }

    for (uint8_t i = 0; i < enc_pass.storage_size; i++) {
        for (uint8_t j = 0; j < enc_pass.storage_pass_len; j++) {
            decrypted_passwords[i][j] = 0x00;
        }
    }
}

// you can use key any length with SHA256, kuznechik use only 32byte
#ifdef USE_SHA256_KEY
uint8_t master_key[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
#ifndef USE_SHA256_KEY
uint8_t master_key[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif

uint8_t count_char_key = 0;

uint8_t crypto_mode = 0;

uint8_t decrypted_mode = 0;

uint8_t red_menu_mode = 0;

// encrypt new password
uint8_t *encrypt_pass_kuzn(const uint8_t *new_pass, const uint8_t new_pass_length) {
    kuz_key_t key;
    w128_t    x;
    uint8_t  *new_pass_enc;
    new_pass_enc = (uint8_t *)malloc(sizeof(uint8_t) * enc_pass.storage_pass_len);

    init_dec_pass();
    kuz_init();

// using master_key for encrypting
#ifdef USE_SHA256_KEY
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, master_key, red_min_len(u_strlen(master_key), MAX_KEY_LEN));
    BYTE result_key[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, result_key);
    red_print_hex(result_key);
    kuz_set_decrypt_key(&key, result_key);
#endif
// if you don't have enough FLASH in MCU
#ifndef USE_SHA256_KEY
    kuz_set_decrypt_key(&key, master_key);
#endif

#ifdef USE_RED_CRY_DEBUG
    print_w128(&x);
#endif
    // encrypting one password
    // it will read only blocks with password, empty blocks will not be encrypted!
    for (uint8_t block_index = 0; block_index < CEILING(new_pass_length, BLOCK_SIZE); block_index++) {
        // reading password's block for encrypting
        for (uint8_t byte_index = 0 + block_index * BLOCK_SIZE; byte_index < BLOCK_SIZE + block_index * BLOCK_SIZE; byte_index++) {
            x.b[byte_index % BLOCK_SIZE] = new_pass[byte_index];
        }
        kuz_encrypt_block(&key, &x);
        // reading encrypted block to return variable
        for (uint8_t byte_index = 0 + block_index * BLOCK_SIZE; byte_index < BLOCK_SIZE + block_index * BLOCK_SIZE; byte_index++) {
            new_pass_enc[byte_index + block_index * BLOCK_SIZE] = x.b[byte_index % BLOCK_SIZE];
        }
#ifdef USE_RED_CRY_DEBUG
        print_w128(&x);
        print_chars_w128(&x);
#endif
    }
    return new_pass_enc;
}

// decrypting passwords
void decrypt_pass_kuzn(void) {
    kuz_key_t key;
    w128_t    x;

    init_dec_pass();
    kuz_init();

// for hashing key, like a KDF algorithm
#ifdef USE_SHA256_KEY
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, master_key, red_min_len(u_strlen(master_key), MAX_KEY_LEN));
    BYTE result_key[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, result_key);
    red_print_hex(result_key);
    kuz_set_decrypt_key(&key, result_key);
#endif
// if you don't have enough FLASH in MCU
#ifndef USE_SHA256_KEY
    kuz_set_decrypt_key(&key, master_key);
#endif
    // decrypting all passwords in storage
    for (uint8_t password_index = 0; password_index < enc_pass.storage_size; password_index++) {
#ifdef USE_RED_CRY_DEBUG
        dprintf("decrypted\t=");
#endif
        for (uint8_t block_index = 0; block_index < enc_pass.storage_pass_len / BLOCK_SIZE; block_index++) {
            uint8_t cnt_zeros = 0;
            // checking is this empty block or not
            for (uint8_t byte_index = 0 + block_index * BLOCK_SIZE; byte_index < BLOCK_SIZE + block_index * BLOCK_SIZE; byte_index++) {
                // if (encrypted_passwords[password_index][byte_index] == 0x00) {
                if (enc_pass.passwords[password_index * enc_pass.storage_pass_len + byte_index] == 0x00) {
                    cnt_zeros++;
                }
            }
            if (cnt_zeros == BLOCK_SIZE) break; // if there are BLOCK_SIZE byte zeros block in password, e.g. password length <32 or <64

            // reading password's block for decrypting
            for (uint8_t byte_index = 0 + block_index * BLOCK_SIZE; byte_index < BLOCK_SIZE + block_index * BLOCK_SIZE; byte_index++) {
                // x.b[byte_index % BLOCK_SIZE] = encrypted_passwords[password_index][byte_index];
                x.b[byte_index % BLOCK_SIZE] = enc_pass.passwords[password_index * enc_pass.storage_pass_len + byte_index];
            }

            // decrypting via Kuznechik
            kuz_decrypt_block(&key, &x);

            // writing decrypted password's block
            for (uint8_t byte_index = 0 + block_index * BLOCK_SIZE; byte_index < BLOCK_SIZE + block_index * BLOCK_SIZE; byte_index++) {
                decrypted_passwords[password_index][byte_index] = x.b[byte_index % BLOCK_SIZE];
            }

#ifdef USE_RED_CRY_DEBUG
            print_w128(&x);
            print_chars_w128(&x);
#endif
        }
    }
};

// function for embedding to "proccess_record_user" for adding crypto features
uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (crypto_mode) {
        switch (keycode) {
            case RED_CRY_M:
                if (record->event.pressed) {
#ifdef USE_RED_CRY_DEBUG
                    red_print_int(master_key);
#endif
                    crypto_mode    = 0;
                    count_char_key = 0;
                    if (eeprom_inited == 1) decrypt_pass_kuzn();
                    decrypted_mode = 1;
                }
                break;
            default:
                // reading key
                return kc_to_ascii(keycode, record, master_key, &count_char_key);
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
            case RED_TEST:
#ifdef TEST_FUNCTIONS_ENABLED
                if (record->event.pressed) {
                    exec_sha256_test();
                    exec_kuznechik_test();
                    exec_aes_test();
                }
#endif
                break;
            case RED_RNG:
                if (record->event.pressed) {
                    // TODO: change seed
                    for (uint8_t i = 0; i < 32; i++) {
                        tap_random_base64();
                    }
                }
                break;
            case RED_MENU:
                if (record->event.pressed) {
                    red_menu_mode = 1; // to enter in menu mode block code above
                }
                break;
            case RED_PASS1:
                if (record->event.pressed && enc_pass.password_count <= 1) {
                    red_send_chars_pass(decrypted_passwords[0]);
                }
                break;
            case RED_PASS2:
                if (record->event.pressed && enc_pass.password_count <= 2) {
                    red_send_chars_pass(decrypted_passwords[1]);
                }
                break;
            case RED_PASS3:
                if (record->event.pressed && enc_pass.password_count <= 3) {
                    red_send_chars_pass(decrypted_passwords[2]);
                }
                break;
            case RED_PASS4:
                if (record->event.pressed && enc_pass.password_count <= 4) {
                    red_send_chars_pass(decrypted_passwords[3]);
                }
                break;
            case RED_PASS5:
                if (record->event.pressed && enc_pass.password_count <= 5) {
                    red_send_chars_pass(decrypted_passwords[4]);
                }
                break;
            case RED_PASS6:
                if (record->event.pressed && enc_pass.password_count <= 6) {
                    red_send_chars_pass(decrypted_passwords[5]);
                }
                break;
            case RED_PASS7:
                if (record->event.pressed && enc_pass.password_count <= 7) {
                    red_send_chars_pass(decrypted_passwords[6]);
                }
                break;
            case RED_PASS8:
                if (record->event.pressed && enc_pass.password_count <= 8) {
                    red_send_chars_pass(decrypted_passwords[7]);
                }
                break;
            case RED_PASS9:
                if (record->event.pressed && enc_pass.password_count <= 9) {
                    red_send_chars_pass(decrypted_passwords[8]);
                }
                break;
            case RED_PASS10:
                if (record->event.pressed && enc_pass.password_count == 10) {
                    red_send_chars_pass(decrypted_passwords[9]);
                }
                break;
                // if you want to add more keys for passwords, copy this `case` and add new keys in red_crypto.h
                // case RED_PASSX: //change X here
                //     if (record->event.pressed) {
                //         red_send_chars_pass(decrypted_passwords[X]); //change X here
                //     }
                //     break;
        }
    } else {
        // by default
        switch (keycode) {
            case RED_CRY_M:
                if (record->event.pressed) {
                    crypto_eeprom_init();
                    crypto_mode = 1; // to enter in crypto mode block code above
                    // clearing readed key
                    for (uint8_t i = 0; i < 32; i++) {
                        master_key[i] = 0;
                    }
                    // dprintf("%d", keycode);
                }
                break;
        }
    }
    return 0;
}
