#include "encryption.h"
#include "project-conf.h"
#include "keycreate.h"
#include "ascon.h"
#if defined (KEYCREATE_H_) && defined (ENCRYPTION_H_)
void encrypt(uint8_t *ciphertext,uint8_t *plaintext, uint64_t pt_len, uint8_t *tag,uint64_t tag_len,uint8_t* ad) {
    ascon_aead128_encrypt(ciphertext, tag, key, iv, ad, plaintext,AD_SIZE,pt_len,TAG_SIZE);
}

void decrypt(uint8_t *plaintext,uint8_t *ciphertext, uint64_t ct_len, uint8_t *etag,uint64_t etag_len,uint8_t* ad) {
  ascon_aead128a_decrypt(plaintext, key, iv, ad, ciphertext,etag,AD_SIZE,ct_len,etag_len);   
}
#endif