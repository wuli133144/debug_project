/* crypto/sm1/sm1.h -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 *  * add by fanjc
 *   *
 *    */

#ifndef HEADER_SM1_H
#define HEADER_SM1_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_SM1
#error SM1 is disabled.
#endif

#include <stddef.h>

#define SM1_ENCRYPT		1
#define SM1_DECRYPT		0

#define SM1_BLOCK_SIZE	16
#define SM1_KEY_SIZE	16
#define SM1_IV_SIZE		SM1_BLOCK_SIZE

#ifdef  __cplusplus
extern "C" {
#endif

	/* This should be a hidden type, but EVP requires that the size be known */
	struct sm1_key_st {
		u_int8_t key_data[2*SM1_BLOCK_SIZE];
		u_int32_t key_size;
	};
	typedef struct sm1_key_st SM1_KEY;

	int SM1_set_encrypt_key(const unsigned char *userKey, const int bits,
				SM1_KEY *key);
	int SM1_set_decrypt_key(const unsigned char *userKey, const int bits,
				SM1_KEY *key);

	void SM1_cbc_encrypt(const unsigned char *in, unsigned char *out,
			    size_t length, const SM1_KEY *key,
				    unsigned char *ivec, const int enc);


#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_SM1_H */

