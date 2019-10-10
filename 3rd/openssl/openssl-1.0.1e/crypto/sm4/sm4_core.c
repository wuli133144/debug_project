/* crypto/sm4/sm4_core.c */
/* 
 */

#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <openssl/sm4.h>
#include <openssl/crypto.h>
#include "sm4_soft.h"

int SM4_set_encrypt_key(const unsigned char *userKey, const int bits,
	SM4_KEY *key)
{
	sm4_setkey_enc((sm4_context *)key->key_data, userKey);
	return 0;
}

int SM4_set_decrypt_key(const unsigned char *userKey, const int bits,
	SM4_KEY *key)
{
	sm4_setkey_dec((sm4_context *)key->key_data, userKey);
	return 0;
}

void SM4_cbc_encrypt(const unsigned char *in, unsigned char *out,
    size_t length, const SM4_KEY *key,
    unsigned char *ivec, const int enc)
{
	sm4_crypt_cbc((sm4_context *)key->key_data, enc, length, ivec, in, out);
	return;
}
