#include <stdint.h>
#include "encryption.h"
#include "keycreate.h"
#include "ascon.h"
#if defined (KEYCREATE_H_) && defined (ENCRYPTION_H_)
void encrypt(uint8_t *ciphertext, uint8_t *plaintext, uint64_t pt_len, uint8_t *tag, uint64_t tag_len, uint8_t* ad) {
    ascon_aead128_encrypt(ciphertext, tag, key, iv, ad, AD_SIZE, plaintext, pt_len, tag_len);
}

void decrypt(uint8_t *plaintext, uint8_t *ciphertext, uint64_t ct_len, uint8_t *etag, uint64_t etag_len, uint8_t* ad) {
    ascon_aead128_decrypt(plaintext, key, iv, ad, AD_SIZE, ciphertext, ct_len, etag, etag_len);
}
#endif