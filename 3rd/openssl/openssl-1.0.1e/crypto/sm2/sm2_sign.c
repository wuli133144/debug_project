/* crypto/sm2/sm2_sign.c */

#include "sm2_locl.h"
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif
#include <openssl/rand.h>
#include <openssl/sm3.h>

/*
 * Output Z, return length of Z
 */
int SM2_sign_pre_process(const EC_KEY *ec, unsigned char *Z)
	{
	int     ret=-1, reason=ERR_R_BIO_LIB;
	const EC_GROUP *group;
	const EC_POINT *pub;

#define ID_LEN	16
	unsigned char lenAndID[ID_LEN + 2] = { \
		0x00, 0x80,
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,\
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
	};
	const EC_POINT *point=NULL;
	BIGNUM *a = BN_new();
	BIGNUM *b = BN_new();
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	BN_CTX  *bctx = BN_CTX_new();
	unsigned char *buff = NULL;
	int len;
	SM3_CTX c;

	group = EC_KEY_get0_group(ec);
	pub = EC_KEY_get0_public_key(ec);

	if (!a || !b || !x || !y || !bctx)
		{
		reason = ERR_R_MALLOC_FAILURE;
		goto err;
		}

	if (!EC_GROUP_get_curve_GFp(group, NULL, a, b, bctx))
		{
		reason = ERR_R_EC_LIB;
		goto err;
		}
	if ((point = EC_GROUP_get0_generator(group)) == NULL)
		{
		reason = ERR_R_EC_LIB;
		goto err;
		}

	buff = OPENSSL_malloc(SM2_size(ec));
	if (!buff)	{
		reason = ERR_R_MALLOC_FAILURE;
		goto err;
	}

	SM3_Init(&c);
	SM3_Update(&c, lenAndID, ID_LEN+2);
	len = BN_bn2bin(a, buff);	SM3_Update(&c, buff, len);
	len = BN_bn2bin(b, buff);	SM3_Update(&c, buff, len);
	EC_POINT_get_affine_coordinates_GFp(group, point, x, y, NULL);
	len = BN_bn2bin(x, buff);	SM3_Update(&c, buff, len);
	len = BN_bn2bin(y, buff);	SM3_Update(&c, buff, len);
	EC_POINT_get_affine_coordinates_GFp(group, pub, x, y, NULL);
	len = BN_bn2bin(x, buff);	SM3_Update(&c, buff, len);
	len = BN_bn2bin(y, buff);	SM3_Update(&c, buff, len);
	SM3_Final(Z, &c);
	OPENSSL_cleanse(&c, sizeof(c)); /* security consideration */

	ret = 0;

err:
	if (!ret)
 		ECerr(EC_F_DO_EC_KEY_PRINT, reason);
	if (bctx)
		BN_CTX_free(bctx);
	if (a) BN_free(a);
	if (b) BN_free(b);
	if (x) BN_free(x);
	if (y) BN_free(y);
	if (buff) OPENSSL_free(buff);
	return ret;
	}

SM2_SIG *SM2_do_sign_ex(const unsigned char *dgst, int dlen,
	const BIGNUM *kinv, const BIGNUM *rp, EC_KEY *eckey)
{
	SM2_DATA *sm2 = sm2_check(eckey);
	if (sm2 == NULL)
		return NULL;
	return sm2->meth->sm2_do_sign(dgst, dlen, kinv, rp, eckey);
}

SM2_SIG *SM2_do_sign(const unsigned char *dgst, int dlen, EC_KEY *eckey)
{
	return SM2_do_sign_ex(dgst, dlen, NULL, NULL, eckey);
}

int SM2_sign_ex(int type, const unsigned char *dgst, int dlen, unsigned char 
	*sig, unsigned int *siglen, const BIGNUM *kinv, const BIGNUM *r, 
	EC_KEY *eckey)
{
	SM2_SIG *s;
	RAND_seed(dgst, dlen);
	s = SM2_do_sign_ex(dgst, dlen, kinv, r, eckey);
	if (s == NULL) {
		*siglen=0;
		return 0;
	}
	*siglen = i2d_SM2_SIG(s, &sig);
	SM2_SIG_free(s);
	return 1;
}

int SM2_sign(int type, const unsigned char *dgst, int dlen, unsigned char 
		*sig, unsigned int *siglen, EC_KEY *eckey)
{
	return SM2_sign_ex(type, dgst, dlen, sig, siglen, NULL, NULL, eckey);
}

int SM2_sign_setup(EC_KEY *eckey, BN_CTX *ctx_in, BIGNUM **kinvp, 
		BIGNUM **rp)
{
	SM2_DATA *sm2 = sm2_check(eckey);
	if (sm2 == NULL)
		return 0;
	return sm2->meth->sm2_sign_setup(eckey, ctx_in, kinvp, rp); 
}
