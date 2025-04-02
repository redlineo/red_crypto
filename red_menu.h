#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef RED_MENU_H
#    define RED_MENU_H

#    include "kc_to_ascii.h"
#    include "red_crypto.h"

#define CLEAR_MENU_MACROS send_string(SS_DOWN(X_LCTL) SS_DOWN(X_A) SS_UP(X_LCTL) SS_UP(X_A) SS_TAP(X_BACKSPACE))

uint8_t draw_red_menu(uint16_t keycode, keyrecord_t *record);

#endif