/* crypto/sm2/sm2_crpt.c */

#include "sm2_locl.h"

int SM2_encrypt(int flen, const unsigned char *from, unsigned char *to, EC_KEY *eckey)
	{
	SM2_DATA *sm2 = sm2_check(eckey);
	if (sm2 == NULL)
		return -1;
#ifdef SM2_TEST
	sm2_hexdump("plain", from, flen);
#endif
	return (sm2->meth->sm2_do_encrypt(flen, from, to, eckey));
	}

int SM2_decrypt(int flen, const unsigned char *from, unsigned char *to, EC_KEY *eckey)
	{
	SM2_DATA *sm2 = sm2_check(eckey);
	if (sm2 == NULL)
		return -1;
#ifdef SM2_TEST
	sm2_hexdump("cipher", from, flen);
#endif
	return (sm2->meth->sm2_do_decrypt( flen, from,to, eckey));
	}
