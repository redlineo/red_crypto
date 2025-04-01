#ifndef RED_UTILS_H
#define RED_UTILS_H
#include QMK_KEYBOARD_H
#include "quantum.h"
#include "red_crypto.h"

void red_print_chars(uint8_t *c);

void red_print_int(uint8_t *c, uint8_t len);

void red_print_hex(uint8_t *h);

size_t red_min_len(size_t a, size_t b);

void red_send_chars_pass(uint8_t *out);

uint8_t *hashing_string(uint8_t *str_to_hash);

uint8_t *red_init_array(uint8_t len, uint8_t val);

uint8_t **red_init_array_2d(uint8_t len_row, uint8_t len_col, uint8_t val);

void red_write_to_storage(void);

void red_read_from_storage(void);

#endif