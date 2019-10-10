/* crypto/evp/m_sm3.c */

#include <stdio.h>
#include "cryptlib.h"

#ifndef OPENSSL_NO_SM3

#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/sm3.h>
#ifndef OPENSSL_NO_RSA
#include <openssl/rsa.h>
#endif
#include "evp_locl.h"

static int init(EVP_MD_CTX *ctx)
	{ return SM3_Init(ctx->md_data); }

static int update(EVP_MD_CTX *ctx,const void *data,size_t count)
	{ return SM3_Update(ctx->md_data,data,count); }

static int final(EVP_MD_CTX *ctx,unsigned char *md)
	{ return SM3_Final(md,ctx->md_data); }

static const EVP_MD sm3_md=
	{
	NID_sm3,
	NID_sm3WithRSAEncryption,
	SM3_DIGEST_LENGTH,
	0,
	init,
	update,
	final,
	NULL,
	NULL,
	EVP_PKEY_RSA_method,
	SM3_CBLOCK,
	sizeof(EVP_MD *)+sizeof(SM3_CTX),
	};

const EVP_MD *EVP_sm3(void)
	{
	return(&sm3_md);
	}
#endif
