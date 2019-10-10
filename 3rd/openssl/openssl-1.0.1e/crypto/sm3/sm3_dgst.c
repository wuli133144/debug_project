/* crypto/sm3/sm3_dgst.c */

#include <stdio.h>
#include "sm3_locl.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

#define OPENSSL_SM3_SOFT

#ifdef OPENSSL_SM3_SOFT
#include "sm3_soft.h"
#endif

int SM3_Init(SM3_CTX *c)
{
	int err = 1;
#ifdef OPENSSL_SM3_SOFT
	sm3_soft_init((sm3_context *)c->priv);
#endif
	return err;
}

int SM3_Update(SM3_CTX *c, const void *data, size_t len)
{
	int err = 1;
#ifdef OPENSSL_SM3_SOFT
	sm3_soft_update((sm3_context *)c->priv, data, len);
#endif
	return err;
}

int SM3_Final(unsigned char *md, SM3_CTX *c)
{
	int err = 1;
#ifdef OPENSSL_SM3_SOFT
	sm3_soft_final((sm3_context *)c->priv, md);
#endif
	return err;
}
