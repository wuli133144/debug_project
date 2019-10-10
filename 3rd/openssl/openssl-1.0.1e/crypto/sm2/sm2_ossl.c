/* crypto/sm2/sm2_ossl.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sm2_locl.h"
#include <openssl/err.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/sm3.h>

static int sm2_sign_setup(EC_KEY *eckey, BN_CTX *ctx_in, BIGNUM **kinvp,
		BIGNUM **rp)
{
	BN_CTX   *ctx = NULL;
	BIGNUM	 *k = NULL, *r = NULL, *order = NULL, *X = NULL;
	EC_POINT *tmp_point=NULL;
	const EC_GROUP *group;
	int 	 ret = 0;

	if (eckey == NULL || (group = EC_KEY_get0_group(eckey)) == NULL)
	{
		SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_PASSED_NULL_PARAMETER);
		return 0;
	}

	if (ctx_in == NULL) {
		if ((ctx = BN_CTX_new()) == NULL)
		{
			SM2err(SM2_F_SM2_SIGN_SETUP,ERR_R_MALLOC_FAILURE);
			return 0;
		}
	}
	else
		ctx = ctx_in;

	k     = BN_new();	/* this value is later returned in *kinvp */
	r     = BN_new();	/* this value is later returned in *rp    */
	order = BN_new();
	X     = BN_new();
	if (!k || !r || !order || !X)
	{
		SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_MALLOC_FAILURE);
		goto err;
	}
	if ((tmp_point = EC_POINT_new(group)) == NULL)
	{
		SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_EC_LIB);
		goto err;
	}
	if (!EC_GROUP_get_order(group, order, ctx))
	{
		SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_EC_LIB);
		goto err;
	}
	
	do
	{
		/* get random k */	
		do
			if (!BN_rand_range(k, order))
			{
				SM2err(SM2_F_SM2_SIGN_SETUP,
				 SM2_R_RANDOM_NUMBER_GENERATION_FAILED);	
				goto err;
			}
		while (BN_is_zero(k));

		/* compute r the x-coordinate of generator * k */
		if (!EC_POINT_mul(group, tmp_point, k, NULL, NULL, ctx))
		{
			SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_EC_LIB);
			goto err;
		}
		if (EC_METHOD_get_field_type(EC_GROUP_method_of(group)) == NID_X9_62_prime_field)
		{
			if (!EC_POINT_get_affine_coordinates_GFp(group,
				tmp_point, X, NULL, ctx))
			{
				SM2err(SM2_F_SM2_SIGN_SETUP,ERR_R_EC_LIB);
				goto err;
			}
		}
#ifndef OPENSSL_NO_EC2M
		else /* NID_X9_62_characteristic_two_field */
		{
			if (!EC_POINT_get_affine_coordinates_GF2m(group,
				tmp_point, X, NULL, ctx))
			{
				SM2err(SM2_F_SM2_SIGN_SETUP,ERR_R_EC_LIB);
				goto err;
			}
		}
#endif
		if (!BN_nnmod(r, X, order, ctx))
		{
			SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_BN_LIB);
			goto err;
		}
	}
	while (BN_is_zero(r));

	/* compute the inverse of k */
/*	if (!BN_mod_inverse(k, k, order, ctx))
	{
		SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_BN_LIB);
		goto err;	
	}
*/	/* clear old values if necessary */
	if (*rp != NULL)
		BN_clear_free(*rp);
	if (*kinvp != NULL) 
		BN_clear_free(*kinvp);
	/* save the pre-computed values  */
	*rp    = r;
	*kinvp = k;
	ret = 1;
err:
	if (!ret)
	{
		if (k != NULL) BN_clear_free(k);
		if (r != NULL) BN_clear_free(r);
	}
	if (ctx_in == NULL) 
		BN_CTX_free(ctx);
	if (order != NULL)
		BN_free(order);
	if (tmp_point != NULL) 
		EC_POINT_free(tmp_point);
	if (X)
		BN_clear_free(X);
	return(ret);
}


static SM2_SIG *sm2_do_sign(const unsigned char *dgst, int dgst_len, 
		const BIGNUM *in_kinv, const BIGNUM *in_r, EC_KEY *eckey)
{
	int     ok = 0, i;
	BIGNUM *kinv=NULL, *s, *m=NULL,*tmp=NULL,*order=NULL;
	const BIGNUM *ckinv;
	BN_CTX     *ctx = NULL;
	const EC_GROUP   *group;
	SM2_SIG  *ret;
	const BIGNUM *priv_key;
	BIGNUM *r,*x=NULL,*a=NULL;	//new added

	group    = EC_KEY_get0_group(eckey);
	priv_key = EC_KEY_get0_private_key(eckey);

	if (group == NULL || priv_key == NULL) {
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_PASSED_NULL_PARAMETER);
		return NULL;
	}

	ret = SM2_SIG_new();
	if (!ret) {
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_MALLOC_FAILURE);
		return NULL;
	}
	s = ret->s;
	r = ret->r;

	if ((ctx = BN_CTX_new()) == NULL || (order = BN_new()) == NULL ||
		(tmp = BN_new()) == NULL || (m = BN_new()) == NULL || 
		(x = BN_new()) == NULL || (a = BN_new()) == NULL)
	{
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_MALLOC_FAILURE);
		goto err;
	}

	if (!EC_GROUP_get_order(group, order, ctx))
	{
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_EC_LIB);
		goto err;
	}
	i = BN_num_bits(order);
	/* Need to truncate digest if it is too long: first truncate whole
	 * bytes.
	 */
	if (8 * dgst_len > i)
		dgst_len = (i + 7)/8;
	if (!BN_bin2bn(dgst, dgst_len, m))
	{
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
		goto err;
	}
	/* If still too long truncate remaining bits with a shift */
	if ((8 * dgst_len > i) && !BN_rshift(m, m, 8 - (i & 0x7)))
	{
		SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
		goto err;
	}
	do
	{
		if (in_kinv == NULL || in_r == NULL)
		{
			if (!sm2_sign_setup(eckey, ctx, &kinv, &x))
			{
				SM2err(SM2_F_SM2_DO_SIGN,ERR_R_EC_LIB);
				goto err;
			}
			ckinv = kinv;
		}
		else
		{
			ckinv  = in_kinv;
			if (BN_copy(x, in_r) == NULL)
			{
				SM2err(SM2_F_SM2_DO_SIGN, ERR_R_MALLOC_FAILURE);
				goto err;
			}
		}
		/* r=(e+x1) mod n */
		if (!BN_mod_add_quick(r, m, x, order))
		{
			SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
			goto err;
		}

		if(BN_is_zero(r) )
			continue;
		BN_add(tmp,r,ckinv);
		if (BN_ucmp(tmp,order) == 0)
			continue;
		if (!BN_mod_mul(tmp, priv_key, r, order, ctx))
		{
			SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
			goto err;
		}
		if (!BN_mod_sub_quick(s, ckinv, tmp, order))
		{
			SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
			goto err;
		}
		BN_one(a);

		if (!BN_mod_add_quick(tmp, priv_key, a, order))
		{
			SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
			goto err;
		}
		/* compute the inverse of 1+dA */
		if (!BN_mod_inverse(tmp, tmp, order, ctx))
		{
			SM2err(SM2_F_SM2_SIGN_SETUP, ERR_R_BN_LIB);
			goto err;	
		}
		if (!BN_mod_mul(s, s, tmp, order, ctx))
		{
			SM2err(SM2_F_SM2_DO_SIGN, ERR_R_BN_LIB);
			goto err;
		}
		if (BN_is_zero(s))
		{
			/* if kinv and r have been supplied by the caller
			 * don't to generate new kinv and r values */
			if (in_kinv != NULL && in_r != NULL)
			{
				SM2err(SM2_F_SM2_DO_SIGN, SM2_R_NEED_NEW_SETUP_VALUES);
				goto err;
			}
		}
		else
			/* s != 0 => we have a valid signature */
			break;
	} while (1);

	ok = 1;
err:
	if (!ok)
	{
		SM2_SIG_free(ret);
		ret = NULL;
	}
	if (ctx)
		BN_CTX_free(ctx);
	if (m)
		BN_clear_free(m);
	if (tmp)
		BN_clear_free(tmp);
	if (order)
		BN_free(order);
	if (kinv)
		BN_clear_free(kinv);
	if (x)
		BN_clear_free(x);
	if (a)
		BN_clear_free(a);
	return ret;
}

static int sm2_do_verify(const unsigned char *dgst, int dgst_len,
		const SM2_SIG *sig, EC_KEY *eckey)
{
	int ret = -1, i;
	BN_CTX   *ctx;
	BIGNUM   *order, *R,  *m, *X, *t;
	EC_POINT *point = NULL;
	const EC_GROUP *group;
	const EC_POINT *pub_key;

	/* check input values */
	if (eckey == NULL || (group = EC_KEY_get0_group(eckey)) == NULL ||
	    (pub_key = EC_KEY_get0_public_key(eckey)) == NULL || sig == NULL)
	{
		SM2err(SM2_F_SM2_DO_VERIFY, SM2_R_MISSING_PARAMETERS);
		return -1;
	}

	ctx = BN_CTX_new();
	if (!ctx)
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_MALLOC_FAILURE);
		return -1;
	}
	BN_CTX_start(ctx);
	order = BN_CTX_get(ctx);	
	R    = BN_CTX_get(ctx);
	t    = BN_CTX_get(ctx);
	m     = BN_CTX_get(ctx);
	X     = BN_CTX_get(ctx);
	if (!X)
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_BN_LIB);
		goto err;
	}
	
	if (!EC_GROUP_get_order(group, order, ctx))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_EC_LIB);
		goto err;
	}

	if (BN_is_zero(sig->r)          || BN_is_negative(sig->r) || 
	    BN_ucmp(sig->r, order) >= 0 || BN_is_zero(sig->s)  ||
	    BN_is_negative(sig->s)      || BN_ucmp(sig->s, order) >= 0)
	{
		SM2err(SM2_F_SM2_DO_VERIFY, SM2_R_BAD_SIGNATURE);
		ret = 0;	/* signature is invalid */
		goto err;
	}

	//t =(r+s) mod n
	if (!BN_mod_add_quick(t, sig->s, sig->r,order))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_BN_LIB);
		goto err;
	}
	if (BN_is_zero(t))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, SM2_R_BAD_SIGNATURE);
		ret = 0;	/* signature is invalid */
		goto err;
	}
	
	//point = s*G+t*PA
	if ((point = EC_POINT_new(group)) == NULL)
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_MALLOC_FAILURE);
		goto err;
	}
	if (!EC_POINT_mul(group, point, sig->s, pub_key, t, ctx))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_EC_LIB);
		goto err;
	}
	if (EC_METHOD_get_field_type(EC_GROUP_method_of(group)) == NID_X9_62_prime_field)
	{
		if (!EC_POINT_get_affine_coordinates_GFp(group,
			point, X, NULL, ctx))
		{
			SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_EC_LIB);
			goto err;
		}
	}
	else /* NID_X9_62_characteristic_two_field */
	{
		if (!EC_POINT_get_affine_coordinates_GF2m(group,
			point, X, NULL, ctx))
		{
			SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_EC_LIB);
			goto err;
		}
	}
 	
	i = BN_num_bits(order);
	/* Need to truncate digest if it is too long: first truncate whole
	 * bytes.
	 */
	if (8 * dgst_len > i)
		dgst_len = (i + 7)/8;
	if (!BN_bin2bn(dgst, dgst_len, m))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_BN_LIB);
		goto err;
	}
	/* If still too long truncate remaining bits with a shift */
	if ((8 * dgst_len > i) && !BN_rshift(m, m, 8 - (i & 0x7)))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_BN_LIB);
		goto err;
	}

	/* R = m + X mod order */
	if (!BN_mod_add_quick(R, m, X, order))
	{
		SM2err(SM2_F_SM2_DO_VERIFY, ERR_R_BN_LIB);
		goto err;
	}

	/*  if the signature is correct R is equal to sig->r */
	ret = (BN_ucmp(R, sig->r) == 0);

err:
	BN_CTX_end(ctx);
	BN_CTX_free(ctx);
	if (point)
		EC_POINT_free(point);

	return ret;
}

static void KDF(const unsigned char *share, unsigned int sharelen,
	unsigned int keylen, unsigned char *outkey)
{
	SM3_CTX ctx;
	unsigned char dgst[SM3_DIGEST_LENGTH] = {0};
	unsigned char *pp = outkey;
	unsigned char u_ct[4];
	unsigned int ct = 1;
	int rlen = (int)keylen;

	while (rlen > 0) {
		u_ct[0] = (unsigned char)(ct>>24);
		u_ct[1] = (unsigned char)(ct>>16);
		u_ct[2] = (unsigned char)(ct>>8);
		u_ct[3] = (unsigned char)ct;
		SM3_Init(&ctx);
		SM3_Update(&ctx, share, sharelen);
		SM3_Update(&ctx, u_ct, sizeof(u_ct));
		SM3_Final(dgst, &ctx);
		OPENSSL_cleanse(&ctx, sizeof(ctx));
		++ct;
		memcpy(pp, dgst, rlen >= SM3_DIGEST_LENGTH ? SM3_DIGEST_LENGTH : rlen);

		rlen -= SM3_DIGEST_LENGTH;
		pp += SM3_DIGEST_LENGTH;
	}
#ifdef SM2_TEST
	sm2_hexdump("t", outkey, keylen);
#endif
}

static int sm2_do_encrypt_ex(unsigned int flen, const unsigned char *from, 
	unsigned char *to, EC_KEY *eckey, BIGNUM *k_in)
{
	const EC_GROUP *group;
	const EC_POINT *pubkey;

	int xylen = 0, olen, ret = -1;
	unsigned int i = 0;
	unsigned char *t = NULL, *temp = NULL;
	unsigned char *c;
	SM3_CTX sm3_ctx;

	BN_CTX *ctx = BN_CTX_new();	/* necessary ? */
	EC_POINT *C1, *S;
	BIGNUM *order = BN_new();
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	BIGNUM *h = BN_new();
	BIGNUM *k = NULL;

	/* check input values */
	if (eckey == NULL || (group = EC_KEY_get0_group(eckey)) == NULL ||
	    (pubkey = EC_KEY_get0_public_key(eckey)) == NULL || to == NULL) {
		SM2err(SM2_F_SM2_DO_ENCRYPT, SM2_R_MISSING_PARAMETERS);
		return -1;
	}

	C1 = EC_POINT_new(group);
	S = EC_POINT_new(group);
	if (!ctx || !order || !C1 || !x || !y || !h || !S) {
		SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_MALLOC_FAILURE);
		goto cleanup;
	}

	if (!EC_GROUP_get_order(group, order, ctx)) {
		SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}

	xylen = EC_GROUP_get_degree(group) / 8;
	olen = 1 + 2 * xylen + flen + SM3_DIGEST_LENGTH;

	RAND_seed(from, flen);
	do {
		if (NULL == k_in) {
			if (NULL == (k = BN_new())) {
				SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_MALLOC_FAILURE);
				goto cleanup;
			}
			/* generate rand number: k */
			do {
				if (!BN_rand_range(k, order)) {
					SM2err(SM2_F_SM2_DO_ENCRYPT, SM2_R_RANDOM_NUMBER_GENERATION_FAILED);
					goto cleanup;
				}
			} while (BN_is_zero(k));
		} else
			k = k_in;

		/* compute C1 */
		if (!EC_POINT_mul(group, C1, k, NULL, NULL, ctx)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}
		if (!EC_POINT_get_affine_coordinates_GFp(group, C1, x, y, ctx)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}	
		to[0] = POINT_CONVERSION_UNCOMPRESSED;
		if (!BN_bn2bin(x, to + 1)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_BN_LIB);
			goto cleanup;
		}
		if (!BN_bn2bin(y, to + 1 + xylen)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_BN_LIB);
			goto cleanup;
		}		
		/* check whether S is the point at infinity */
		if (!EC_GROUP_get_cofactor(group, h, ctx)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}

		if (!EC_POINT_mul(group, S, NULL, pubkey, h, ctx) ||
			EC_POINT_is_at_infinity(group, S)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}
		/* compute t=KDF(x||y, flen) */
		if (!EC_POINT_mul(group, S, NULL, pubkey, k, ctx)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}
		if (!EC_POINT_get_affine_coordinates_GFp(group, S, x, y, ctx)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_EC_LIB);
			goto cleanup;
		}

		if (!temp)
			temp = (unsigned char *)OPENSSL_malloc(2 * xylen);
		if (!BN_bn2bin(x, temp)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_BN_LIB);
			goto cleanup;
		}
		if (!BN_bn2bin(y, temp + xylen)) {
			SM2err(SM2_F_SM2_DO_ENCRYPT, ERR_R_BN_LIB);
			goto cleanup;
		}
		if (!t)
			t = (unsigned char *)OPENSSL_malloc(flen);
		memset(t, 0, flen);
		KDF(temp, 2 * xylen, flen, t);
		for(i = 0; i < flen && t[i] == 0; ++i);
	} while(i == flen);

	/* compute C3 */
	c = to + 1 + 2 * xylen;
	SM3_Init(&sm3_ctx);
	SM3_Update(&sm3_ctx, temp, xylen);
	SM3_Update(&sm3_ctx, from, flen);
	SM3_Update(&sm3_ctx, temp + xylen, xylen);
	SM3_Final(c, &sm3_ctx);
	OPENSSL_cleanse(&sm3_ctx, sizeof(sm3_ctx));

	/* compute C2 */
	c += SM3_DIGEST_LENGTH;
	for (i = 0; i < flen; ++i)
		c[i] = from[i] ^ t[i];
	ret = olen;

cleanup:
	if (ctx) BN_CTX_free(ctx);
	if (order) BN_free(order);
	if (!k_in) BN_free(k);
	if (x) BN_free(x);
	if (y) BN_free(y);
	if (h) BN_free(h);
	if (C1) EC_POINT_free(C1);
	if (S) EC_POINT_free(S);
	if (temp) OPENSSL_free(temp);
	if (t) OPENSSL_free(t);

	return ret;
}

static int sm2_do_encrypt(int flen, const unsigned char *from,
	unsigned char *to, EC_KEY *eckey)
{
	return sm2_do_encrypt_ex(flen, from, to, eckey, NULL);
}

static int sm2_do_decrypt(int flen, const unsigned char *from,
	unsigned char *to, EC_KEY *eckey)
{
	const EC_GROUP *group;
	const BIGNUM *prikey;

	int xylen = 0, olen, ret = -1;
	BN_CTX *ctx = BN_CTX_new();
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	BIGNUM *h = BN_new();
	EC_POINT *C1 = NULL, *S = NULL;

	unsigned int i = 0;
	unsigned char *t = NULL, *temp = NULL;
	const unsigned char *p;
	SM3_CTX sm3_ctx;
	unsigned char u[SM3_DIGEST_LENGTH] = {0,};

	/* check input values */
	if (eckey == NULL || (group = EC_KEY_get0_group(eckey)) == NULL ||
	    (prikey = EC_KEY_get0_private_key(eckey)) == NULL || to == NULL) {
		SM2err(SM2_F_SM2_DO_DECRYPT, SM2_R_MISSING_PARAMETERS);
		return -1;
	}

	/*  */
	if (POINT_CONVERSION_UNCOMPRESSED != from[0]) {
		SM2err(SM2_F_SM2_DO_DECRYPT, SM2_R_UNSUPPORT_COMPRESSED);
		goto cleanup;
	}

	C1 = EC_POINT_new(group);
	S = EC_POINT_new(group);
	if (!ctx || !C1 || !x || !y || !h || !S) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_MALLOC_FAILURE);
		goto cleanup;
	}	

	xylen = EC_GROUP_get_degree(group) / 8;
	olen = flen - 1 - 2 * xylen - SM3_DIGEST_LENGTH;

	/* get C1 from cipher */
	if (!BN_bin2bn(from + 1, xylen, x)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_BN_LIB);
		goto cleanup;
	}
	if (!BN_bin2bn(from + 1 + xylen, xylen, y)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_BN_LIB);
		goto cleanup;
	}
	if (!EC_POINT_set_affine_coordinates_GFp(group, C1, x, y, ctx)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}
	/* check whether C1 is on curve */
	if (!EC_POINT_is_on_curve(group, C1, ctx)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}

	/* check whether S=[h]C1 is the point at infinity */
	if (!EC_GROUP_get_cofactor(group, h, ctx)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}
	if (!EC_POINT_mul(group, S, NULL, C1, h, ctx) ||
		EC_POINT_is_at_infinity(group, S)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}

	/* compute [d_B]C1 */
	if (!EC_POINT_mul(group, S, NULL, C1, prikey, ctx)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}
	if (!EC_POINT_get_affine_coordinates_GFp(group, S, x, y, ctx)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_EC_LIB);
		goto cleanup;
	}

	if (!temp)
		temp = (unsigned char *)OPENSSL_malloc(2 * xylen);
	if (!BN_bn2bin(x, temp)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_BN_LIB);
		goto cleanup;
	}
	if (!BN_bn2bin(y, temp + xylen)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, ERR_R_BN_LIB);
		goto cleanup;
	}

	/* compute t=KDF(x||y, olen) */
	if (!t)
		t = (unsigned char *)OPENSSL_malloc(olen);
	memset(t, 0, olen);

	KDF(temp, 2 * xylen, olen, t);
	/* t must be not all of zero */
	for(i = 0; i < flen && t[i] == 0; ++i);
	if (i == flen) {
		SM2err(SM2_F_SM2_DO_DECRYPT, SM2_R_DECRYPT_FAILED);
		goto cleanup;
	}

	p = from + 1 + 2 * xylen + SM3_DIGEST_LENGTH;
	for (i = 0; i < olen; ++i)
		to[i] = p[i] ^ t[i];

	/* compute u=Hash(x||M'||y), and u must be equal to C3 */
	SM3_Init(&sm3_ctx);
	SM3_Update(&sm3_ctx, temp, xylen);
	SM3_Update(&sm3_ctx, to, olen);
	SM3_Update(&sm3_ctx, temp + xylen, xylen);
	SM3_Final(u, &sm3_ctx);
	OPENSSL_cleanse(&sm3_ctx, sizeof(sm3_ctx));

	p -= SM3_DIGEST_LENGTH;
	if (memcmp(u, p, SM3_DIGEST_LENGTH)) {
		SM2err(SM2_F_SM2_DO_DECRYPT, SM2_R_DECRYPT_FAILED);
		goto cleanup;
	}
	ret = olen;

cleanup:
	if (ctx) BN_CTX_free(ctx);
	if (x) BN_free(x);
	if (y) BN_free(y);
	if (h) BN_free(h);
	if (C1) EC_POINT_free(C1);
	if (S) EC_POINT_free(S);
	if (temp) OPENSSL_free(temp);
	if (t) OPENSSL_free(t);

	return ret;
}

static SM2_METHOD openssl_sm2_meth = {
	"OpenSSL SM2 method",
	sm2_do_sign,
	sm2_sign_setup,
	sm2_do_verify,
	sm2_do_encrypt,
	sm2_do_decrypt,
	0,    /* flags    */
	NULL  /* app_data */
};

const SM2_METHOD *SM2_OpenSSL(void)
{
	return &openssl_sm2_meth;
}
