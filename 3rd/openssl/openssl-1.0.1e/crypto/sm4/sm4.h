/* crypto/sm1/sm1.h -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 *  * add by fanjc
 *   *
 *    */

#ifndef HEADER_SM4_H
#define HEADER_SM4_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_SM4
#error SM4 is disabled.
#endif

#include <stddef.h>

#define SM4_ENCRYPT		1
#define SM4_DECRYPT		0

#define SM4_BLOCK_SIZE	16
#define SM4_KEY_SIZE	16
#define SM4_IV_SIZE		SM4_BLOCK_SIZE

#ifdef  __cplusplus
extern "C" {
#endif

	/* This should be a hidden type, but EVP requires that the size be known */
	
	struct sm4_key_st {
		unsigned char key_data[16*SM4_KEY_SIZE];
	};
	typedef struct sm4_key_st SM4_KEY;

	
	int SM4_set_encrypt_key(const unsigned char *userKey, const int bits,
				SM4_KEY *key);
	int SM4_set_decrypt_key(const unsigned char *userKey, const int bits,
				SM4_KEY *key);

	void SM4_cbc_encrypt(const unsigned char *in, unsigned char *out,
			    size_t length, const SM4_KEY *key,
				    unsigned char *ivec, const int enc);

#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_SM1_H */

