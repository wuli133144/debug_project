/* crypto/sm2/sm2test.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/opensslconf.h> /* To see if OPENSSL_NO_SM2 is defined */

#ifdef OPENSSL_NO_SM2
int main(int argc, char * argv[])
	{
	puts("Elliptic curves are disabled.");
	return 0;
	}
#else

#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/sm2.h>
#include <openssl/sm3.h>
#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif
#include <openssl/err.h>
#include <openssl/rand.h>

static const char rnd_seed[] = "string to make the random number generator "
	"think it has entropy";

RAND_METHOD	fake_rand;
const RAND_METHOD *old_rand;

static int fbytes_counter = 0;
static const char *numbers[4] = {
	"3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8",
	"59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21",
	"3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8",
	"59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21"
	};

static int _fbytes(unsigned char *buf, int num)
	{
	int	ret;
	BIGNUM	*tmp = NULL;

	if (fbytes_counter >= 4)
		return 0;
	tmp = BN_new();
	if (!tmp)
		return 0;
	if (!BN_hex2bn(&tmp, numbers[fbytes_counter]))
		{
		BN_free(tmp);
		return 0;
		}
	//printf("counter:%d, size:%d, num:%d\n", fbytes_counter, BN_num_bytes(tmp), num);
	fbytes_counter ++;
	if (num != BN_num_bytes(tmp) || !BN_bn2bin(tmp, buf))
		ret = 0;
	else 
		ret = 1;
	if (tmp)
		BN_free(tmp);
	return ret;
	}

/* functions to change the RAND_METHOD */
static int sm2_change_rand(void)
	{
	/* save old rand method */
	if ((old_rand = RAND_get_rand_method()) == NULL)
		return 0;

	fake_rand.seed    = old_rand->seed;
	fake_rand.cleanup = old_rand->cleanup;
	fake_rand.add     = old_rand->add;
	fake_rand.status  = old_rand->status;
	/* use own random function */
	fake_rand.bytes      = _fbytes;
	fake_rand.pseudorand = old_rand->bytes;
	/* set new RAND_METHOD */
	if (!RAND_set_rand_method(&fake_rand))
		return 0;
	return 1;
	}

static int sm2_restore_rand(void)
	{
	if (!RAND_set_rand_method(old_rand))
		return 0;
	else
		return 1;
	}

static const char Z_exp[] = {
	"B2E14C5C79C6DF5B85F4FE7ED8DB7A262B9DA7E07CCB0EA9F4747B8CCDA8A4F3"
};
static const char Dgst_exp[] = {
	"F0B43E94BA45ACCAACE692ED534382EB17E6AB5A19CE7B31F4486FDFC0D28640"
};
static const char *sig_exp[2] = {
	"F5A03B0648D2C4630EEAC513E1BB81A15944DA3827D5B74143AC7EACEEE720B3",
	"B1B6AA29DF212FD8763182BC0D421CA1BB9038FD1F7F42D4840B69C485BBC1AA"
};

static void hex2str(const unsigned char *val, int len, char *to)
{
	int i, k = 0;
	for(i=0; i<len; i++)
		k += sprintf(to+k, "%02X", val[i]);
	to[k] = 0;
}

static int sm2_sign_test(BIO *out)
	{
	int	ret = 0;
	int nid = NID_sm2;
	const char message[] = "message digest";
	unsigned char Z[SM3_DIGEST_LENGTH] = {0,};
	unsigned char digest[SM3_DIGEST_LENGTH];
	unsigned int  dgst_len = 0;
	EVP_MD_CTX md_ctx;
	EC_KEY    *key = NULL;
	SM2_SIG *signature = NULL;
	char res[64], *sig_r = NULL, *sig_s = NULL;

	/* create the key */
	if ((key = EC_KEY_new_by_curve_name(nid)) == NULL) {
		BIO_printf(out, "create EC_KEY ");
		goto err;
	}
	if (!EC_KEY_generate_key(key)) {
		BIO_printf(out, "generate key ");
		goto err;
	}

	/* pre-process: compute Z_A */
	if (SM2_sign_pre_process(key, Z)) {
		BIO_printf(out, "sign pre-process ");
		goto err;
	}

	hex2str(Z, SM3_DIGEST_LENGTH, res);
	if (strcmp(res, Z_exp)) {
		BIO_printf(out, "Z_A:  %s\n", res);
		BIO_printf(out, "Exp:  %s\n", Z_exp);
		BIO_printf(out, "Compute Z ");
		goto err;
	}

	EVP_MD_CTX_init(&md_ctx);
	/* get the message digest */
	EVP_DigestInit(&md_ctx, EVP_sm3_sm2());
	EVP_DigestUpdate(&md_ctx, Z, SM3_DIGEST_LENGTH);
	EVP_DigestUpdate(&md_ctx, (const void*)message, strlen(message));
	EVP_DigestFinal(&md_ctx, digest, &dgst_len);

	hex2str(digest, dgst_len, res);
	if (strcmp(res, Dgst_exp)) {
		BIO_printf(out, "Dgst: %s\n", res);
		BIO_printf(out, "Exp:  %s\n", Dgst_exp);
		BIO_printf(out, "digest ");
		goto err;
	}

	/* create the signature */
	signature = SM2_do_sign(digest, dgst_len, key);
	if (signature == NULL)
		goto err;
	sig_r = BN_bn2hex(signature->r);
	sig_s = BN_bn2hex(signature->s);
	if (strcmp(sig_r, sig_exp[0]) || strcmp(sig_s, sig_exp[1])) {
		BIO_printf(out, "sig_r:%s\n", sig_r);
		BIO_printf(out, "Exp:  %s\n", sig_exp[0]);
		BIO_printf(out, "sig_s:%s\n", sig_s);
		BIO_printf(out, "Exp:  %s\n", sig_exp[1]);
		BIO_printf(out, "signature ");
		goto err;
	}

	/* verify the signature */
	if (SM2_do_verify(digest, 32, signature, key) != 1)
		goto err;

	BIO_printf(out, "test sign and verify ok\n\n");
	ret = 1;
err:
	if (!ret)
		BIO_printf(out, " failed\n\n");
	if (sig_r)	OPENSSL_free(sig_r);
	if (sig_s)	OPENSSL_free(sig_s);
	if (key)
		EC_KEY_free(key);
	if (signature)
		SM2_SIG_free(signature);
	EVP_MD_CTX_cleanup(&md_ctx);

	return ret;
	}

int sm2_crypt_test(BIO *out)
	{
	int	ret = 0;
	int nid = NID_sm2;
#define MSG_LEN	19
	/* "encryption standard" */
	const unsigned char message[MSG_LEN] = { \
		0x65,0x6E,0x63,0x72,0x79,0x70,0x74,0x69, \
		0x6F,0x6E,0x20,0x73,0x74,0x61,0x6E,0x64, \
		0x61,0x72,0x64 \
		};
	unsigned char *c = NULL;
	unsigned char *p = NULL;
	int len, n;
	EC_KEY    *key = NULL;

	/* create the key */
	if ((key = EC_KEY_new_by_curve_name(nid)) == NULL) {
		BIO_printf(out, "create EC_KEY failed");
		goto err;
	}
	if (!EC_KEY_generate_key(key)) {
		BIO_printf(out, "generate key failed");
		goto err;
	}

	len = EC_GROUP_get_degree(EC_KEY_get0_group(key)) / 8;
	n = 1 + 2 * len + MSG_LEN + SM3_DIGEST_LENGTH;
	c = OPENSSL_malloc(n + 1);
	/* encrypt  */
	len = SM2_encrypt(MSG_LEN, message, c, key);
	if (len <= 0) {
		BIO_printf(out, "encrypt failed\n");
		goto err;
	}

	p = OPENSSL_malloc(MSG_LEN + 1);
	/* decrypt */
	len = SM2_decrypt(len, c, p, key);
	if (len <= 0) {
		BIO_printf(out, "decrypt failed\n");
		goto err;
	}

	if (memcmp(message, p, MSG_LEN)) {
		char res[512];
		hex2str(c, len, res);
		BIO_printf(out, "cipher:\n%s\n", res);
		hex2str(p, len, res);
		BIO_printf(out, "out-plain:\n%s\n", res);
		hex2str(message, len, res);
		BIO_printf(out, "in-plain:\n%s\n", res);
		BIO_printf(out, "test encrypt and decrypt failed\n");
		goto err;
	}

	BIO_printf(out, "test encrypt and decrypt ok\n");
	ret = 1;
err:
	if (key)
		EC_KEY_free(key);

	if (c) OPENSSL_free(c);
	if (p) OPENSSL_free(p);

	return ret;
	}

int main(void)
	{
	int 	ret = 1;
	BIO	*out;

	out = BIO_new_fp(stdout, BIO_NOCLOSE);
	
	/* enable memory leak checking unless explicitly disabled */
	if (!((getenv("OPENSSL_DEBUG_MEMORY") != NULL) && 
		(0 == strcmp(getenv("OPENSSL_DEBUG_MEMORY"), "off"))))
		{
		CRYPTO_malloc_debug_init();
		CRYPTO_set_mem_debug_options(V_CRYPTO_MDEBUG_ALL);
		}
	else
		{
		/* OPENSSL_DEBUG_MEMORY=off */
		CRYPTO_set_mem_debug_functions(0, 0, 0, 0, 0);
		}
	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);

	ERR_load_crypto_strings();

	/* initialize the prng */
	RAND_seed(rnd_seed, sizeof(rnd_seed));

	/* set own rand method */
	if (!sm2_change_rand())
		goto err;

	/* the tests */
	if (!sm2_sign_test(out))  goto err;
	if (!sm2_crypt_test(out)) goto err;
	
	ret = 0;
err:	
	sm2_restore_rand();

	if (ret) 	
		BIO_printf(out, "\nSM2 test failed\n");
	else 
		BIO_printf(out, "\nSM2 test passed\n");
	if (ret)
		ERR_print_errors(out);
	CRYPTO_cleanup_all_ex_data();
	ERR_remove_thread_state(NULL);
	ERR_free_strings();
	CRYPTO_mem_leaks(out);
	if (out != NULL)
		BIO_free(out);
	return ret;
	}	
#endif
