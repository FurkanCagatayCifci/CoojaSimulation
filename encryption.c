#include "ascon-c/crypto_aead/asconaead128/armv6/ascon.h"
#define KEY_SIZE 16  // 128-bit anahtar
#define IV_SIZE 16   // 128-bit IV
#define TAG_SIZE 16  // 128-bit doğrulama etiketi

uint8_t key[KEY_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
uint8_t iv[IV_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
const uint8_t *ad = NULL;  // No associated data
uint64_t ad_len = 0;

void encrypt(uint8_t *plaintext, uint64_t pt_len, uint8_t *ciphertext, uint8_t *tag) {
    ascon_aead_encrypt(ciphertext, tag, plaintext, pt_len, ad, ad_len, iv, key);
}

void decrypt(uint8_t *ciphertext, uint64_t ct_len, uint8_t *plaintext, uint8_t *tag) {
  return ascon_aead_decrypt(plaintext, tag, ciphertext, ct_len, ad, ad_len, iv, key);   
}