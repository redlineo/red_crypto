// Author: redlineo, romanov.pd@gmail.com

#include QMK_KEYBOARD_H
#include "quantum.h"

#ifndef KC_TO_ASCII_H
#    define KC_TO_ASCII_H

uint8_t kc_to_ascii(uint16_t keycode, keyrecord_t *record, uint8_t readed_key[], uint8_t *count_char_key);

/****************************** ASCII TABLE ******************************/

// clang-format off
#define ASCII_NON_PRINTABLE 0x00        //
#define ASCII_SOH           0x01        //
#define ASCII_STX           0x02        //
#define ASCII_ETX           0x03        //
#define ASCII_EOT           0x04        //
#define ASCII_ENQ           0x05        //
#define ASCII_ACK           0x06        //
#define ASCII_BEL           0x07        //
#define ASCII_BS            0x08        //
#define ASCII_HT            0x09        //
#define ASCII_LF            0x0A        //
#define ASCII_VT            0x0B        //
#define ASCII_FF            0x0C        //
#define ASCII_CR            0x0D        //
#define ASCII_SO            0x0E        //
#define ASCII_SI            0x0F        //

#define ASCII_DLE           0x10        //
#define ASCII_DC1           0x11        //
#define ASCII_DC2           0x12        //
#define ASCII_DC3           0x13        //
#define ASCII_DC4           0x14        //
#define ASCII_NAK           0x15        //
#define ASCII_SYN           0x16        //
#define ASCII_ETB           0x17        //
#define ASCII_CAN           0x18        //
#define ASCII_EM            0x19        //
#define ASCII_SUB           0x1A        //
#define ASCII_ESC           0x1B        //
#define ASCII_FS            0x1C        //
#define ASCII_GS            0x1D        //
#define ASCII_RS            0x1E        //
#define ASCII_US            0x1F        //

#define ASCII_SPACE         0x20        // ' '
#define ASCII_UP_1          0x21        // !
#define ASCII_UP_QUOTE      0x22        // "
#define ASCII_UP_3          0x23        // #
#define ASCII_UP_4          0x24        // $
#define ASCII_UP_5          0x25        // %
#define ASCII_UP_7          0x26        // &
#define ASCII_LOW_QUOTE     0x27        // '
#define ASCII_UP_9          0x28        // (
#define ASCII_UP_0          0x29        // )
#define ASCII_UP_8          0x2A        // *
#define ASCII_UP_EQUAL      0x2B        // +
#define ASCII_LOW_COMMA     0x2C        // ,
#define ASCII_LOW_MINUS     0x2D        // -
#define ASCII_LOW_DOT       0x2E        // .
#define ASCII_LOW_SLASH     0x2F        // /

#define ASCII_LOW_0         0x30        // 0
#define ASCII_LOW_1         0x31        // 1
#define ASCII_LOW_2         0x32        // 2
#define ASCII_LOW_3         0x33        // 3
#define ASCII_LOW_4         0x34        // 4
#define ASCII_LOW_5         0x35        // 5
#define ASCII_LOW_6         0x36        // 6
#define ASCII_LOW_7         0x37        // 7
#define ASCII_LOW_8         0x38        // 8
#define ASCII_LOW_9         0x39        // 9
#define ASCII_UP_SEMICOLON  0x3A        // :
#define ASCII_LOW_SEMICOLON 0x3B        // ;
#define ASCII_UP_COMMA      0x3C        // <
#define ASCII_LOW_EQUAL     0x3D        // =
#define ASCII_UP_DOT        0x3E        // >
#define ASCII_UP_SLASH      0x3F        // ?

#define ASCII_UP_2          0x40        // @
#define ASCII_UP_A          0x41        // A
#define ASCII_UP_B          0x42        // B
#define ASCII_UP_C          0x43        // C
#define ASCII_UP_D          0x44        // D
#define ASCII_UP_E          0x45        // E
#define ASCII_UP_F          0x46        // F
#define ASCII_UP_G          0x47        // G
#define ASCII_UP_H          0x48        // H
#define ASCII_UP_I          0x49        // I
#define ASCII_UP_J          0x4A        // J
#define ASCII_UP_K          0x4B        // K
#define ASCII_UP_L          0x4C        // L
#define ASCII_UP_M          0x4D        // M
#define ASCII_UP_N          0x4E        // N
#define ASCII_UP_O          0x4F        // O

#define ASCII_UP_P          0x50        // P
#define ASCII_UP_Q          0x51        // Q
#define ASCII_UP_R          0x52        // R
#define ASCII_UP_S          0x53        // S
#define ASCII_UP_T          0x54        // T
#define ASCII_UP_U          0x55        // U
#define ASCII_UP_V          0x56        // V
#define ASCII_UP_W          0x57        // W
#define ASCII_UP_X          0x58        // X
#define ASCII_UP_Y          0x59        // Y
#define ASCII_UP_Z          0x5A        // Z
#define ASCII_LOW_LEFT_BR   0x5B        // [
#define ASCII_LOW_BACKSLASH 0x5C        /* \        */
#define ASCII_LOW_RIGHT_BR  0x5D        // ]
#define ASCII_UP_6          0x5E        // ^
#define ASCII_UP_MINUS      0x5F        // _

#define ASCII_GRAVE         0x60        // `
#define ASCII_LOW_A         0x61        // a
#define ASCII_LOW_B         0x62        // b
#define ASCII_LOW_C         0x63        // c
#define ASCII_LOW_D         0x64        // d
#define ASCII_LOW_E         0x65        // e
#define ASCII_LOW_F         0x66        // f
#define ASCII_LOW_G         0x67        // g
#define ASCII_LOW_H         0x68        // h
#define ASCII_LOW_I         0x69        // i
#define ASCII_LOW_J         0x6A        // j
#define ASCII_LOW_K         0x6B        // k
#define ASCII_LOW_L         0x6C        // l
#define ASCII_LOW_M         0x6D        // m
#define ASCII_LOW_N         0x6E        // n
#define ASCII_LOW_O         0x6F        // o

#define ASCII_LOW_P         0x70        // p
#define ASCII_LOW_Q         0x71        // q
#define ASCII_LOW_R         0x72        // r
#define ASCII_LOW_S         0x73        // s
#define ASCII_LOW_T         0x74        // t
#define ASCII_LOW_U         0x75        // u
#define ASCII_LOW_V         0x76        // v
#define ASCII_LOW_W         0x77        // w
#define ASCII_LOW_X         0x78        // x
#define ASCII_LOW_Y         0x79        // y
#define ASCII_LOW_Z         0x7A        // z
#define ASCII_UP_LEFT_BR    0x7B        // {
#define ASCII_UP_BACKSLASH  0x7C        // |
#define ASCII_UP_RIGHT_BR   0x7D        // }
#define ASCII_UP_HASH       0x7E        // ~
#define ASCII_DEL           0x7F        // DEL


#endif
