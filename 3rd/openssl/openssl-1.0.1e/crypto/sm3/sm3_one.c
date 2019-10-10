/* crypto/sm3/sm3_one.c */

#include <stdio.h>
#include <string.h>
#include <openssl/sm3.h>
#include <openssl/crypto.h>

unsigned char *SM3(const unsigned char *d, size_t n, unsigned char *md)
{
	SM3_CTX c;
	static unsigned char m[SM3_DIGEST_LENGTH];

	if (md == NULL) md = m;
	if (!SM3_Init(&c))
		return NULL;
	SM3_Update(&c,d,n);
	SM3_Final(md,&c);
	OPENSSL_cleanse(&c,sizeof(c)); /* security consideration */
	return(md);
}

