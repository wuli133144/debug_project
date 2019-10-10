/* crypto/sm2/sm2.h */
#ifndef HEADER_SM2_H
#define HEADER_SM2_H

#include <openssl/opensslconf.h>

#ifdef OPENSSL_NO_SM2
#error SM2 is disabled.
#endif

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/ossl_typ.h>
#ifndef OPENSSL_NO_DEPRECATED
#include <openssl/bn.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SM2_BIGNUM_SIZE	32

typedef struct SM2_SIG_st
{
	BIGNUM *r;
	BIGNUM *s;
} SM2_SIG;

/* This four functions are the same with ECDSA */

/** Allocates and initialize a SM2_SIG structure
 *  \return pointer to a SM2_SIG structure or NULL if an error occurred
 */
SM2_SIG *SM2_SIG_new(void);

/** frees a SM2_SIG structure
 *  \param  sig  pointer to the SM2_SIG structure
 */
void	  SM2_SIG_free(SM2_SIG *sig);

/** DER encode content of ECDSA_SIG object (note: this function modifies *pp
 *  (*pp += length of the DER encoded signature)).
 *  \param  sig  pointer to the ECDSA_SIG object
 *  \param  pp   pointer to a unsigned char pointer for the output or NULL
 *  \return the length of the DER encoded ECDSA_SIG object or 0 
 */
int	  i2d_SM2_SIG(const SM2_SIG *sig, unsigned char **pp);

/** Decodes a DER encoded ECDSA signature (note: this function changes *pp
 *  (*pp += len)). 
 *  \param  sig  pointer to ECDSA_SIG pointer (may be NULL)
 *  \param  pp   memory buffer with the DER encoded signature
 *  \param  len  length of the buffer
 *  \return pointer to the decoded ECDSA_SIG structure (or NULL)
 */
SM2_SIG *d2i_SM2_SIG(SM2_SIG **sig, const unsigned char **pp, long len);

int SM2_sign_pre_process(const EC_KEY *ec, unsigned char *Z);

/** Computes the SM2 signature of the given hash value using
 *  the supplied private key and returns the created signature.
 *  \param  dgst      pointer to the hash value
 *  \param  dgst_len  length of the hash value
 *  \param  eckey     EC_KEY object containing a private EC key
 *  \return pointer to a SM2_SIG structure or NULL if an error occurred
 */
SM2_SIG *SM2_do_sign(const unsigned char *dgst,int dgst_len,EC_KEY *eckey);

/** Computes SM2 signature of a given hash value using the supplied
 *  private key (note: sig must point to SM2_size(eckey) bytes of memory).
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  kinv     BIGNUM with a pre-computed inverse k (optional)
 *  \param  rp       BIGNUM with a pre-computed rp value (optioanl), 
 *                   see SM2_sign_setup
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return pointer to a SM2_SIG structure or NULL if an error occurred
 */
SM2_SIG *SM2_do_sign_ex(const unsigned char *dgst, int dgstlen, 
		const BIGNUM *kinv, const BIGNUM *rp, EC_KEY *eckey);

/** Verifies that the supplied signature is a valid SM2
 *  signature of the supplied hash value using the supplied public key.
 *  \param  dgst      pointer to the hash value
 *  \param  dgst_len  length of the hash value
 *  \param  sig       SM2_SIG structure
 *  \param  eckey     EC_KEY object containing a public EC key
 *  \return 1 if the signature is valid, 0 if the signature is invalid
 *          and -1 on error
 */
int SM2_do_verify(const unsigned char *dgst, int dgst_len,
		const SM2_SIG *sig, EC_KEY* eckey);

const SM2_METHOD *SM2_OpenSSL(void);

/** Sets the default SM2 method
 *  \param  meth  new default SM2_METHOD
 */
void	  SM2_set_default_method(const SM2_METHOD *meth);

/** Returns the default SM2 method
 *  \return pointer to SM2_METHOD structure containing the default method
 */
const SM2_METHOD *SM2_get_default_method(void);

/** Sets method to be used for the SM2 operations
 *  \param  eckey  EC_KEY object
 *  \param  meth   new method
 *  \return 1 on success and 0 otherwise 
 */
int 	  SM2_set_method(EC_KEY *eckey, const SM2_METHOD *meth);

/** Returns the maximum length of the DER encoded signature
 *  \param  eckey  EC_KEY object
 *  \return numbers of bytes required for the DER encoded signature
 */
int	  SM2_size(const EC_KEY *eckey);

/** Returns the crypto length
 *  \param  eckey  EC_KEY object
 *  \param  ilen     length of in data
 *  \param  enc     encrypt (1) or decrypt (0)
 *  \return numbers of bytes required for encrypt or decrypt which depends on 'enc'
 */
int SM2_crpt_size(const EC_KEY *eckey, int ilen, int enc);

/** Precompute parts of the signing operation
 *  \param  eckey  EC_KEY object containing a private EC key
 *  \param  ctx    BN_CTX object (optional)
 *  \param  kinv   BIGNUM pointer for the inverse of k
 *  \param  rp     BIGNUM pointer for x coordinate of k * generator
 *  \return 1 on success and 0 otherwise
 */
int 	  SM2_sign_setup(EC_KEY *eckey, BN_CTX *ctx, BIGNUM **kinv, 
		BIGNUM **rp);

/** Computes SM2 signature of a given hash value using the supplied
 *  private key (note: sig must point to SM2_size(eckey) bytes of memory).
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  sig      memory for the DER encoded created signature
 *  \param  siglen   pointer to the length of the returned signature
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return 1 on success and 0 otherwise
 */
int	  SM2_sign(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, EC_KEY *eckey);


/** Computes SM2 signature of a given hash value using the supplied
 *  private key (note: sig must point to SM2_size(eckey) bytes of memory).
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value to sign
 *  \param  dgstlen  length of the hash value
 *  \param  sig      buffer to hold the DER encoded signature
 *  \param  siglen   pointer to the length of the returned signature
 *  \param  kinv     BIGNUM with a pre-computed inverse k (optional)
 *  \param  rp       BIGNUM with a pre-computed rp value (optioanl), 
 *                   see SM2_sign_setup
 *  \param  eckey    EC_KEY object containing a private EC key
 *  \return 1 on success and 0 otherwise
 */
int	  SM2_sign_ex(int type, const unsigned char *dgst, int dgstlen, 
		unsigned char *sig, unsigned int *siglen, const BIGNUM *kinv,
		const BIGNUM *rp, EC_KEY *eckey);

/** Verifies that the given signature is valid SM2 signature
 *  of the supplied hash value using the specified public key.
 *  \param  type     this parameter is ignored
 *  \param  dgst     pointer to the hash value 
 *  \param  dgstlen  length of the hash value
 *  \param  sig      pointer to the DER encoded signature
 *  \param  siglen   length of the DER encoded signature
 *  \param  eckey    EC_KEY object containing a public EC key
 *  \return 1 if the signature is valid, 0 if the signature is invalid
 *          and -1 on error
 */
int 	  SM2_verify(int type, const unsigned char *dgst, int dgstlen, 
		const unsigned char *sig, int siglen, EC_KEY *eckey);

int SM2_encrypt(int flen, const unsigned char *from, unsigned char *to, EC_KEY *eckey);
int SM2_decrypt(int flen, const unsigned char *from, unsigned char *to, EC_KEY *eckey);

/* the standard ex_data functions */
int 	  SM2_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new 
		*new_func, CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int 	  SM2_set_ex_data(EC_KEY *d, int idx, void *arg);
void 	  *SM2_get_ex_data(EC_KEY *d, int idx);


/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
void ERR_load_SM2_strings(void);

/* Error codes for the SM2 functions. */

/* Function codes. */
#define SM2_F_SM2_CHECK				 104
#define SM2_F_SM2_DATA_NEW_METHOD			 100
#define SM2_F_SM2_DO_DECRYPT				 106
#define SM2_F_SM2_DO_ENCRYPT				 105
#define SM2_F_SM2_DO_SIGN				 101
#define SM2_F_SM2_DO_VERIFY				 102
#define SM2_F_SM2_SIGN_SETUP			 103

/* Reason codes. */
#define SM2_R_BAD_SIGNATURE				 100
#define SM2_R_DATA_TOO_LARGE_FOR_KEY_SIZE		 101
#define SM2_R_DECRYPT_FAILED			 102
#define SM2_R_MISSING_PARAMETERS			 103
#define SM2_R_NEED_NEW_SETUP_VALUES			 106
#define SM2_R_NON_FIPS_METHOD				 107
#define SM2_R_RANDOM_NUMBER_GENERATION_FAILED		 104
#define SM2_R_SIGNATURE_MALLOC_FAILED			 105
#define SM2_R_UNSUPPORT_COMPRESSED			 108

//#define SM2_TEST
#ifdef SM2_TEST
inline static void sm2_hexdump(const char *desc, const unsigned char *val, int len)
{
	int i;
	printf("%s(%d):\n", desc, len);
	for(i=0; i<len; i++) {
		if (i && !(i&0x1F))
			printf("\n");
		printf("%02X", val[i]);
	}
	printf("\n");
}
#endif

#ifdef  __cplusplus
}
#endif
#endif
