#define DEF_HASH_SHA1   1
#define DEF_HASH_SHA224 2
#define DEF_HASH_SHA256 3
#define DEF_HASH_SHA384 4
#define DEF_HASH_SHA512 5
#define DEF_HASH_RMD160 6
int hash(unsigned char *message, unsigned int len, unsigned char *digest, unsigned int uiMode);
int hmac(unsigned char key[], unsigned long key_len, unsigned char data[], unsigned long data_len, unsigned char mac[], unsigned long mac_len, unsigned int uiMode);
void KDF1(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode);
void KDF2(unsigned char *ucKey, unsigned int uiKeyLen, unsigned char *ucData, unsigned int uiDataLen, unsigned int uiNewKeyLen, unsigned char *ucNewKey, unsigned int uiMode);
