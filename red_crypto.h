// Author: redlineo, romanov.pd@gmail.com

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef RED_CRYPTO_H
#    define RED_CRYPTO_H

#    include "kc_to_ascii.h"

// #ifdef USE_RED_KUZNECHIK_128
// #include "kuznechik_128bit.h"
// #endif

#    ifdef USE_RED_KUZNECHIK_8
#        include "kuznechik_8bit.h"
#        include "sha256.h"
#    endif

#    ifdef USE_RED_AES_256
#        include "aes.h"
#    endif

enum red_crypto_keys {
    RED_CRY_M = SAFE_RANGE + 0x80, // added safe range offset
    RED_RNG,
    RED_TEST,
    RED_PASS1,
    RED_PASS2
};

uint8_t crypto_process_record_user(uint16_t keycode, keyrecord_t *record, const uint8_t encrypted_passwords[2][16]);

#endif
