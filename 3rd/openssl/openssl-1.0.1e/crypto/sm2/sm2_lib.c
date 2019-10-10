/* crypto/sm2/sm2_lib.c */

#include <string.h>
#include "sm2_locl.h"
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif
#ifndef OPENSSL_NO_SM3
#include <openssl/sm3.h>
#endif
#include <openssl/err.h>
#include <openssl/bn.h>
#ifdef OPENSSL_FIPS
#include <openssl/fips.h>
#endif

const char SM2_version[]="SM2" OPENSSL_VERSION_PTEXT;

static const SM2_METHOD *default_SM2_method = NULL;

static void *sm2_data_new(void);
static void *sm2_data_dup(void *);
static void  sm2_data_free(void *);

void SM2_set_default_method(const SM2_METHOD *meth)
{
	default_SM2_method = meth;
}

const SM2_METHOD *SM2_get_default_method(void)
{
	if(!default_SM2_method) 
		{
#ifdef OPENSSL_FIPS
		if (FIPS_mode())
			return FIPS_sm2_openssl();
		else
			return SM2_OpenSSL();
#else
		default_SM2_method = SM2_OpenSSL();
#endif
		}
	return default_SM2_method;
}

int SM2_set_method(EC_KEY *eckey, const SM2_METHOD *meth)
{
	SM2_DATA *sm2;

	sm2 = sm2_check(eckey);

	if (sm2 == NULL)
		return 0;

#ifndef OPENSSL_NO_ENGINE
	if (sm2->engine)
	{
		ENGINE_finish(sm2->engine);
		sm2->engine = NULL;
	}
#endif
        sm2->meth = meth;

        return 1;
}

static SM2_DATA *SM2_DATA_new_method(ENGINE *engine)
{
	SM2_DATA *ret;

	ret=(SM2_DATA *)OPENSSL_malloc(sizeof(SM2_DATA));
	if (ret == NULL)
	{
		SM2err(SM2_F_SM2_DATA_NEW_METHOD, ERR_R_MALLOC_FAILURE);
		return(NULL);
	}

	ret->init = NULL;

	ret->meth = SM2_get_default_method();
	ret->engine = engine;
#ifndef OPENSSL_NO_ENGINE
	if (!ret->engine)
		ret->engine = ENGINE_get_default_SM2();
	if (ret->engine)
	{
		ret->meth = ENGINE_get_SM2(ret->engine);
		if (!ret->meth)
		{
			SM2err(SM2_F_SM2_DATA_NEW_METHOD, ERR_R_ENGINE_LIB);
			ENGINE_finish(ret->engine);
			OPENSSL_free(ret);
			return NULL;
		}
	}
#endif

	ret->flags = ret->meth->flags;
	CRYPTO_new_ex_data(CRYPTO_EX_INDEX_SM2, ret, &ret->ex_data);
	return(ret);
}

static void *sm2_data_new(void)
{
	return (void *)SM2_DATA_new_method(NULL);
}

static void *sm2_data_dup(void *data)
{
	SM2_DATA *r = (SM2_DATA *)data;

	/* XXX: dummy operation */
	if (r == NULL)
		return NULL;

	return sm2_data_new();
}

static void sm2_data_free(void *data)
{
	SM2_DATA *r = (SM2_DATA *)data;

#ifndef OPENSSL_NO_ENGINE
	if (r->engine)
		ENGINE_finish(r->engine);
#endif
	CRYPTO_free_ex_data(CRYPTO_EX_INDEX_SM2, r, &r->ex_data);

	OPENSSL_cleanse((void *)r, sizeof(SM2_DATA));

	OPENSSL_free(r);
}

SM2_DATA *sm2_check(EC_KEY *key)
{
	SM2_DATA *sm2_data;
 
	void *data = EC_KEY_get_key_method_data(key, sm2_data_dup,
					sm2_data_free, sm2_data_free);
	if (data == NULL)
	{
		sm2_data = (SM2_DATA *)sm2_data_new();
		if (sm2_data == NULL)
			return NULL;
		data = EC_KEY_insert_key_method_data(key, (void *)sm2_data,
			   sm2_data_dup, sm2_data_free, sm2_data_free);
		if (data != NULL)
			{
			/* Another thread raced us to install the key_method
			 * data and won. */
			sm2_data_free(sm2_data);
			sm2_data = (SM2_DATA *)data;
			}
	}
	else
		sm2_data = (SM2_DATA *)data;
#ifdef OPENSSL_FIPS
	if (FIPS_mode() && !(sm2_data->flags & SM2_FLAG_FIPS_METHOD)
			&& !(EC_KEY_get_flags(key) & EC_FLAG_NON_FIPS_ALLOW))
		{
		SM2err(SM2_F_SM2_CHECK, SM2_R_NON_FIPS_METHOD);
		return NULL;
		}
#endif

	return sm2_data;
}

int SM2_size(const EC_KEY *r)
{
	int ret,i;
	ASN1_INTEGER bs;
	BIGNUM	*order=NULL;
	unsigned char buf[4];
	const EC_GROUP *group;

	if (r == NULL)
		return 0;
	group = EC_KEY_get0_group(r);
	if (group == NULL)
		return 0;

	if ((order = BN_new()) == NULL) return 0;
	if (!EC_GROUP_get_order(group,order,NULL))
	{
		BN_clear_free(order);
		return 0;
	} 
	i=BN_num_bits(order);
	bs.length=(i+7)/8;
	bs.data=buf;
	bs.type=V_ASN1_INTEGER;
	/* If the top bit is set the asn1 encoding is 1 larger. */
	buf[0]=0xff;	

	i=i2d_ASN1_INTEGER(&bs,NULL);
	i+=i; /* r and s */
	ret=ASN1_object_size(1,i,V_ASN1_SEQUENCE);
	BN_clear_free(order);
	return(ret);
}

int SM2_crpt_size(const EC_KEY *r, int ilen, int enc)
{
	const EC_GROUP *group = EC_KEY_get0_group(r);
	int ret, xylen = EC_GROUP_get_degree(group) / 8;

	if (enc)
		ret = 1 + xylen + xylen + ilen + SM3_DIGEST_LENGTH;
	else
		ret = ilen - 1 - xylen - xylen - SM3_DIGEST_LENGTH;
	return (ret);
}


int SM2_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	     CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func)
{
	return CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_SM2, argl, argp,
				new_func, dup_func, free_func);
}

int SM2_set_ex_data(EC_KEY *d, int idx, void *arg)
{
	SM2_DATA *sm2;
	sm2 = sm2_check(d);
	if (sm2 == NULL)
		return 0;
	return(CRYPTO_set_ex_data(&sm2->ex_data,idx,arg));
}

void *SM2_get_ex_data(EC_KEY *d, int idx)
{
	SM2_DATA *sm2;
	sm2 = sm2_check(d);
	if (sm2 == NULL)
		return NULL;
	return(CRYPTO_get_ex_data(&sm2->ex_data,idx));
}
