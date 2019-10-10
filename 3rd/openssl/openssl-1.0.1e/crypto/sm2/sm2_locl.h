/* crypto/sm2/sm2_locl.h */

#ifndef HEADER_SM2_LOCL_H
#define HEADER_SM2_LOCL_H

#include <openssl/sm2.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct sm2_method 
	{
	const char *name;
	SM2_SIG *(*sm2_do_sign)(const unsigned char *dgst, int dgst_len, 
			const BIGNUM *inv, const BIGNUM *rp, EC_KEY *eckey);
	int (*sm2_sign_setup)(EC_KEY *eckey, BN_CTX *ctx, BIGNUM **kinv, 
			BIGNUM **r);
	int (*sm2_do_verify)(const unsigned char *dgst, int dgst_len, 
			const SM2_SIG *sig, EC_KEY *eckey);
	int (*sm2_do_encrypt)(int flen, const unsigned char *from,
			   unsigned char *to, EC_KEY *eckey);
	int (*sm2_do_decrypt)(int flen, const unsigned char *from,
			   unsigned char *to, EC_KEY *eckey);
#if 0
	int (*init)(EC_KEY *eckey);
	int (*finish)(EC_KEY *eckey);
#endif
	int flags;
	char *app_data;
	};

/* If this flag is set the SM2 method is FIPS compliant and can be used
 * in FIPS mode. This is set in the validated module method. If an
 * application sets this flag in its own methods it is its responsibility
 * to ensure the result is compliant.
 */

#define SM2_FLAG_FIPS_METHOD	0x1

typedef struct sm2_data_st {
	/* EC_KEY_METH_DATA part */
	int (*init)(EC_KEY *);
	/* method (SM2) specific part */
	ENGINE	*engine;
	int	flags;
	const SM2_METHOD *meth;
	CRYPTO_EX_DATA ex_data;
} SM2_DATA;

/** sm2_check
 * checks whether ECKEY->meth_data is a pointer to a SM2_DATA structure
 * and if not it removes the old meth_data and creates a SM2_DATA structure.
 * \param  eckey pointer to a EC_KEY object
 * \return pointer to a SM2_DATA structure
 */
SM2_DATA *sm2_check(EC_KEY *eckey);

#ifdef  __cplusplus
}
#endif

#endif /* HEADER_ECS_LOCL_H */
