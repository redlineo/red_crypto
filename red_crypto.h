// Author: redlineo, romanov.pd@gmail.com

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef RED_CRYPTO_H
#    define RED_CRYPTO_H

// includes of red_crypto
#    include "kc_to_ascii.h"
#    include "utils.h"
#    include "red_menu.h"

// #ifdef USE_RED_KUZNECHIK_128
// #include "kuznechik_128bit.h"
// #endif

#    ifdef USE_RED_KUZNECHIK_8
#        include "kuznechik_8bit.h"
#        ifdef USE_SHA256_KEY
#            include "sha256.h"
#        endif
#    endif

#    ifdef USE_RED_AES_256
#        include "aes.h"
#    endif

#    define MAX_KEY_LEN 128
#    define BLOCK_SIZE 16

#    ifndef INIT_STORAGE_SIZE
#        define INIT_STORAGE_SIZE 4
#    endif

#    ifndef INIT_STORAGE_PASS_LEN
#        define INIT_STORAGE_PASS_LEN 128
#    endif

#    ifdef INIT_STORAGE_PASS_LEN
#        if (INIT_STORAGE_PASS_LEN % 16 != 0)
#            error "Password length must be multiply of 16 (16,32,64...)"
#        endif
#    endif

// example for encrypted 64 byte password
//  const uint8_t red_enc_pass[STORAGE_SIZE][STORAGE_PASS_LEN] = {
//      {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//       0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
//  };

enum red_crypto_keys {
    RED_CRY_M = SAFE_RANGE + 0x80, // added safe range offset
    RED_MENU,
    RED_RNG,
    RED_TEST,
    RED_LOCK, // TODO: add autolock feature
    RED_PASS1,
    RED_PASS2,
    RED_PASS3,
    RED_PASS4,
    RED_PASS5,
    RED_PASS6,
    RED_PASS7,
    RED_PASS8,
    RED_PASS9,
    RED_PASS10
};



uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record);
void    encrypt_pass_kuzn(void);
void    crypto_eeprom_init(void);

#endif
