

#include QMK_KEYBOARD_H
#include "quantum.h"


#ifndef RED_CRYPTO_H
#define RED_CRYPTO_H

#include "ascii_table.c"

// #ifdef USE_RED_KUZNECHIK_128
// #include "kuznechik_128bit.h"
// #endif
#ifdef USE_RED_KUZNECHIK_8
#include "kuznechik_8bit.h"
#endif

#ifdef USE_RED_AES_256
#include "aes.h"
#endif

#ifdef USE_RED_SHA_256
#include "sha256.h"
#endif


enum red_crypto_keys {
    GET_PASS = SAFE_RANGE,
    RED_PASS1,
    RED_PASS2,
    RED_RNG,
    RED_TEST
};

void crypto_process_record_user(uint16_t keycode, keyrecord_t *record, const uint8_t encrypted_passwords[2][16]);


#endif


