/* crypto/sm3/sm3.h */

#ifndef HEADER_SM3_H
#define HEADER_SM3_H

#include <openssl/e_os2.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef OPENSSL_NO_SM3
#error SM3 is disabled.
#endif

#define SM3_LONG unsigned long
#define SM3_DIGEST_LENGTH 32

#define SM3_CBLOCK	64
#define SM3_PRIV_ST_LEN	256

typedef struct SM3state_st
{
	unsigned char priv[SM3_PRIV_ST_LEN];
} SM3_CTX;

int SM3_Init(SM3_CTX *c);
int SM3_Update(SM3_CTX *c, const void *data, size_t len);
int SM3_Final(unsigned char *md, SM3_CTX *c);
unsigned char *SM3(const unsigned char *d, size_t n, unsigned char *md);

#endif
