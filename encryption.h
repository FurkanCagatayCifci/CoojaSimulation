#define ENCRYPTION_H_
void encrypt(uint8_t *ciphertext,uint8_t *plaintext, uint64_t pt_len, uint8_t *tag,uint64_t tag_len,uint8_t* ad);
void decrypt(uint8_t *plaintext,uint8_t *ciphertext, uint64_t ct_len, uint8_t *etag,uint64_t etag_len,uint8_t* ad);