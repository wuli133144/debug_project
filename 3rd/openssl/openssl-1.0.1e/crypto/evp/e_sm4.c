/* crypto/evp/e_sm4.c */
/* add by fanjc
 */

#include <stdio.h>
#include "cryptlib.h"
#ifndef OPENSSL_NO_SM1
#include <openssl/evp.h>
#include <openssl/objects.h>
#include "evp_locl.h"
#include <openssl/sm4.h>

static int sm4_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
			const unsigned char *iv, int enc);
static int sm4_ctrl(EVP_CIPHER_CTX *c, int type, int arg, void *ptr);

static int sm4_128_cbc_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out,
			  const unsigned char *in, size_t inl)
{
	SM4_cbc_encrypt(in, out, inl, ctx->cipher_data, ctx->iv, ctx->encrypt);
	return 1;
}

BLOCK_CIPHER_def_cbc(sm4_128,SM4_KEY,NID_sm4_128,SM4_BLOCK_SIZE,SM4_KEY_SIZE,SM4_IV_SIZE,
		     0, sm4_init_key,NULL,
		     NULL,
		     NULL,sm4_ctrl)

static int sm4_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key,
			const unsigned char *iv, int enc)
	{
	int ret;
	if (enc) {
		ret=SM4_set_encrypt_key(key, ctx->key_len * 8, ctx->cipher_data); 
	} else {
		ret=SM4_set_decrypt_key(key, ctx->key_len * 8, ctx->cipher_data); 
	}
	memcpy(ctx->iv, iv, SM4_IV_SIZE);
	if(ret != 0)
		{
		return 0;
		}
	return 1;
	}

static int sm4_ctrl(EVP_CIPHER_CTX *c, int type, int arg, void *ptr)
	{
	return 1;
	}

#endif
