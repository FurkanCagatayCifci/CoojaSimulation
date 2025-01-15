#include "keycreate.h"
#ifdef KEYCREATE_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
void* ascon_init() {
    // Initialize random number generator
    srand((unsigned int)time(NULL));

    // Define buffer sizes for key, IV, tag, and associated data
    size_t key_size = 16;  // 128-bit key
    size_t iv_size = 16;   // 128-bit IV
    size_t tag_size = 8;   // 64-bit tag
    size_t associated_data_size = 32;  // Example associated data size

    // Allocate memory for key, IV, tag, and associated data
    uint8_t key[KEY_SIZE];
    uint8_t iv[IV_SIZE];
    uint8_t tag[TAG_SIZE];
    uint8_t associated_data[AD_SIZE];

    // Generate random bytes for each component
    generate_random_bytes(key, KEY_SIZE);
    generate_random_bytes(iv, IV_SIZE);
    generate_random_bytes(tag, TAG_SIZE);
    generate_random_bytes(ad, AD_SIZE);
    return ;
}
#endif