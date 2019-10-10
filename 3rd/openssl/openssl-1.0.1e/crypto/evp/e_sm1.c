/* crypto/evp/e_sm1.c */
/* add by fanjc
 */

#include <stdio.h>
#include "cryptlib.h"
#ifndef OPENSSL_NO_SM1
#include <openssl/evp.h>
#include <openssl/objects.h>
#include "evp_locl.h"
#include <openssl/sm1.h>

static int sm1_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
			const unsigned char *iv, int enc);
static int sm1_ctrl(EVP_CIPHER_CTX *c, int type, int arg, void *ptr);

static int sm1_128_cbc_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out,
			  const unsigned char *in, size_t inl)
{
	SM1_cbc_encrypt(in, out, inl, ctx->cipher_data, ctx->iv, ctx->encrypt);
	return 1;
}

/*
BLOCK_CIPHER_def_cbc() declare 'sm1_128_cbc' as follow,

static const EVP_CIPHER sm1_128_cbc = {
	NID_sm1_128_cbc,
	SM1_BLOCK_SIZE,
	SM1_KEY_SIZE,
	SM1_IV_SIZE,
	0 | EVP_CIPH_CBC_MODE,
	sm1_init_key,
	sm1_128_cbc_cipher,
	NULL,
	sizeof(SM1_KEY),
	NULL,
	NULL,
	sm1_ctrl,
	NULL
};
const EVP_CIPHER *EVP_sm1_128_cbc(void)
{
	return &sm1_128_cbc;
}
*/
BLOCK_CIPHER_def_cbc(sm1_128,SM1_KEY,NID_sm1_128,SM1_BLOCK_SIZE,SM1_KEY_SIZE,SM1_IV_SIZE,
		     0, sm1_init_key,NULL,
		     NULL,
		     NULL,sm1_ctrl)

static int sm1_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
			const unsigned char *iv, int enc)
	{
	int ret;
	if (enc) {
		ret=SM1_set_encrypt_key(key, ctx->key_len * 8, ctx->cipher_data); 
	} else {
		ret=SM1_set_decrypt_key(key, ctx->key_len * 8, ctx->cipher_data); 
	}
	memcpy(ctx->iv, iv, SM1_IV_SIZE);
	if(ret != 0)
		{
		return 0;
		}
	return 1;
	}

static int sm1_ctrl(EVP_CIPHER_CTX *c, int type, int arg, void *ptr)
	{
	return 1;
	}

#endif
