#include "utils.h"

void red_print_chars(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        if (c[i] == 0x00) break;
        dprintf("%c", c[i]);
    }
    dprintf("\n");
};

void red_print_int(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        dprintf("%d", c[i]);
    }
    dprintf("\n");
};

void red_print_hex(uint8_t *h) {
    for (uint8_t i = 0; i < 32; i++) {
        dprintf("%x ", h[i]);
    }
    dprintf("\n");
};

size_t red_min_len(size_t a, size_t b) {
    if (a < b)
        return a;
    else
        return b;
}

void red_send_chars_pass(uint8_t *out) {
    for (uint8_t i = 0; i < INIT_STORAGE_PASS_LEN && out[i] != 0; i++) {
        send_char((char)out[i]);
    }
};

// EEPROM utils

void red_write_to_storage(void) {
    // uint8_t res = 0;
    for (uint16_t raw_index = 0; raw_index < STORAGE_SIZE; raw_index++) {
        // res = EEPROM_WriteDataByte(0x0000 + raw_index, enc_pass.raw[raw_index]);
        EEPROM_WriteDataByte(0x0040 + raw_index, enc_pass.raw[raw_index]);
    }
    // return res;
}

void red_read_from_storage(void) {
    for (uint16_t raw_index = 0; raw_index < STORAGE_SIZE; raw_index++) {
        enc_pass.raw[raw_index] = EEPROM_ReadDataByte(0x0040 + raw_index);
    }
}
