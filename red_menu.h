#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef RED_MENU_H
#    define RED_MENU_H

#    include "kc_to_ascii.h"
#    include "red_crypto.h"

uint8_t draw_red_menu(uint16_t keycode, keyrecord_t *record);

#endif