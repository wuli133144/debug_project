/* crypto/sm2/sm2_vrf.c */

#include "sm2_locl.h"
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

/* returns
 *      1: correct signature
 *      0: incorrect signature
 *     -1: error
 */
int SM2_do_verify(const unsigned char *dgst, int dgst_len, 
		const SM2_SIG *sig, EC_KEY *eckey)
	{
	SM2_DATA *sm2 = sm2_check(eckey);
	if (sm2 == NULL)
		return 0;
	return sm2->meth->sm2_do_verify(dgst, dgst_len, sig, eckey);
	}

/* returns
 *      1: correct signature
 *      0: incorrect signature
 *     -1: error
 */
int SM2_verify(int type, const unsigned char *dgst, int dgst_len,
		const unsigned char *sigbuf, int sig_len, EC_KEY *eckey)
 	{
	SM2_SIG *s;
	int ret=-1;

	s = SM2_SIG_new();
	if (s == NULL) return(ret);
	if (d2i_SM2_SIG(&s, &sigbuf, sig_len) == NULL) goto err;
	ret=SM2_do_verify(dgst, dgst_len, s, eckey);
err:
	SM2_SIG_free(s);
	return(ret);
	}
