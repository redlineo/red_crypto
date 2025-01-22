// Author: redlineo, romanov.pd@gmail.com

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef RED_CRYPTO_H
#    define RED_CRYPTO_H

// includes of red_crypto
#    include "kc_to_ascii.h"
#    include "utils.h"

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

#    ifndef STORAGE_SIZE
#        define STORAGE_SIZE 4
#    endif

#    ifdef STORAGE_PASS_LEN
#        if STORAGE_PASS_LEN % 16 != 0
#            error Storage password length must be a multiple of 16
#        endif
#    endif

#    ifndef STORAGE_PASS_LEN
#        define STORAGE_PASS_LEN 64
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
    RED_RNG,
    RED_TEST,
    RED_PASS1,
    RED_PASS2,
    RED_PASS3,
    RED_PASS4,
    RED_PASS5,
    RED_PASS6,
    RED_PASS7,
    RED_PASS8,
    RED_PASS9,
    RED_PASS10,
    RED_PASS11,
    RED_PASS12
};

uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record, const uint8_t encrypted_passwords[STORAGE_SIZE][STORAGE_PASS_LEN]);

#endif
