#include "utils.h"

void red_print_chars(uint8_t *c) {
    for (uint8_t i = 0; i < 32; i++) {
        if (c[i] == 0x00) break;
        dprintf("%c", c[i]);
    }
    dprintf("\n");
};

void red_print_int(uint8_t *c, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
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

uint8_t *red_init_array(uint8_t len, uint8_t val) {
    uint8_t *arr = (uint8_t *)malloc(sizeof(uint8_t) * len);
    for (uint8_t i = 0; i < len; i++) {
        arr[i] = val;
    }
    return arr;
}

uint8_t **red_init_array_2d(uint8_t len_row, uint8_t len_col, uint8_t val) {
    uint8_t **arr = (uint8_t **)malloc(sizeof(uint8_t *) * len_row);
    for (uint8_t i = 0; i < len_row; i++) {
        arr[i] = (uint8_t *)malloc(sizeof(uint8_t) * len_col);
        for (uint8_t j = 0; j < len_col; j++) {
            arr[i][j] = val;
        }
    }
    return arr;
}

// count printable symbols of ASCII in string with safety check of max length
uint8_t red_strlen_printable(uint8_t *str_printable, uint8_t max_len) {
    uint8_t result = 0;
    while (result < max_len && *str_printable != '\0') {
        result++;
        str_printable++;
    }
    return result;
}

#ifdef USE_SHA256_KEY
// hashing string to 32byte with sha256
uint8_t *hashing_string(uint8_t *str_to_hash) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, str_to_hash, red_strlen_printable(str_to_hash, MAX_KEY_LEN));
    // BYTE result_key[SHA256_BLOCK_SIZE];
    BYTE *result_key = red_init_array(SHA256_BLOCK_SIZE, 0x00);
    sha256_final(&ctx, result_key);
#    ifdef USE_RED_CRY_DEBUG
    red_print_hex(result_key);
#    endif
    return result_key;
}
#endif

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
