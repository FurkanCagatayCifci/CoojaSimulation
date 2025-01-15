#define KEY_SIZE 16
#define IV_SIZE 16
#define TAG_SIZE 8
#define AD_SIZE 32
#define KEYCREATE_H_ "keycreate.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
static uint8_t key[16];
static uint8_t iv[16];
static uint8_t tag[8];
static uint8_t ad[32];
void* ascon_init();
void generate_random_bytes(uint8_t *buffer, size_t length);
void generate_random_bytes(uint8_t *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = rand() % 256;
    }
}