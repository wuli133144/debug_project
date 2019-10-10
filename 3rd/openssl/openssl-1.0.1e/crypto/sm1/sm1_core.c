/* crypto/sm1/sm1_core.c */
/*
 */

#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <openssl/sm1.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/sm/fm_def.h>
#include <openssl/sm/fm_cpc_pub.h>
#include <openssl/sm/sm_pub.h>

//#define SM1_TEST

struct gmcard_block {
	int dev_type;
	void *dev_handle;
	void *open_dev;
	void *close_dev;
	void *enc_func;
	void *dec_func;
	void *get_errinfo;
} global_sm1_key = {0, NULL, NULL, NULL, NULL, NULL, NULL};

typedef unsigned int (fm_OpenDev) (
	FM_U8 *pu8Id,
	FM_U32 u32Type,
	FM_U32 u32Flag,
	FM_HANDLE *hDev
	);

typedef unsigned int (fm_CloseDev) (
	FM_HANDLE hDev
	);

typedef unsigned int (fm_Encrypt) (
	FM_HANDLE hDev,
	FM_HKEY hKey,
	FM_U32 u32Alg,
	FM_U32 u32WorkMode,
	FM_U8 *pu8InBuf,
	FM_U32 u32InLen,
	FM_U8 *pu8OutBuf,
	FM_U32 *pu32OutLen,
	FM_U8 *pu8Key,
	FM_U32 u32KeyLen,
	FM_U8 *pu8IV,
	FM_U32 u32IVLen
	);

typedef unsigned int (fm_Decrypt) (
	FM_HANDLE hDev,
	FM_HKEY hKey,
	FM_U32 u32Alg,
	FM_U32 u32WorkMode,
	FM_U8 *pu8InBuf,
	FM_U32 u32InLen,
	FM_U8 *pu8OutBuf,
	FM_U32 *pu32OutLen,
	FM_U8 *pu8Key,
	FM_U32 u32KeyLen,
	FM_U8 *pu8IV,
	FM_U32 u32IVLen);

typedef unsigned int (fm_ErrInfo) (
    FM_U32 u32LanFlag,
    FM_U32 u32ErrCode,
    FM_U32 *pu32Len,
    FM_S8 *ps8Info
    );

int Load_and_Open_fmdev(int subtype)
{
	void *plib = NULL;
	FM_U32 u32Ret = FME_OK;
	FM_U8 u8Id = 0;
	FM_U32 dev_type;

#ifdef SM1_TEST
	printf("%s() Load dymanic lib [%s]\n", __func__, FM_API_LIB);
#endif

	/* maybe there are other types, we can get information for Fisherman */
	switch(subtype)
	{
	case dev_type_pci_1_2:
		dev_type = FM_DEV_TYPE_SJK0810;
		break;
	case dev_type_pci_2_0:
		dev_type = FM_DEV_TYPE_PCI_2_0X;
		break;
	case dev_type_pcie_1_0:
		dev_type = FM_DEV_TYPE_PCIE_1_0X;
		break;
	case dev_type_pcie_2_0:
		dev_type = FM_DEV_TYPE_PCIE_2_0X;
		break;
	default:
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_UNKNOWN_SMCARD_TYPE);
		return -1;
	}

	plib = dlopen(FM_API_LIB, RTLD_NOW | RTLD_GLOBAL);
	if(NULL == plib) {
#ifdef SM1_TEST
		printf("dlopen err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_DL_FAILURE);
		return -2;
	}

	global_sm1_key.open_dev = dlsym(plib, "FM_CPC_OpenDevice");
	if(NULL == global_sm1_key.open_dev) {
#ifdef SM1_TEST
		printf("get open_dev err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_SYM_FAILURE);
		return -3;
	}

	global_sm1_key.close_dev = dlsym(plib, "FM_CPC_CloseDevice");
	if(NULL == global_sm1_key.close_dev) {
#ifdef SM1_TEST
		printf("get close_dev err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_SYM_FAILURE);
		return -3;
	}

	global_sm1_key.enc_func = dlsym(plib, "FM_CPC_Encrypt");
	if(NULL == global_sm1_key.enc_func) {
#ifdef SM1_TEST
		printf("get enc_func err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_SYM_FAILURE);
		return -3;
	}

	global_sm1_key.dec_func = dlsym(plib, "FM_CPC_Decrypt");
	if(NULL == global_sm1_key.dec_func) {
#ifdef SM1_TEST
		printf("get dec_func err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_SYM_FAILURE);
		return -3;
	}

	global_sm1_key.get_errinfo = dlsym(plib, "FM_CPC_GetErrInfo");
	if(NULL == global_sm1_key.get_errinfo) {
#ifdef SM1_TEST
		printf("get get_errinfo err\n");
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_LOAD_SYM_FAILURE);
		return -3;
	}

	/* open device */
	u32Ret = ((fm_OpenDev *)(global_sm1_key.open_dev))(&u8Id, dev_type,
				(FM_OPEN_MULTITHREAD | FM_OPEN_MULTIPROCESS),
				&(global_sm1_key.dev_handle));
	if (FME_OK != u32Ret)
	{
#ifdef SM1_TEST
		FM_U32 len = 128;
		FM_S8 aBuf[128];
		((fm_ErrInfo *)(global_sm1_key.get_errinfo))(1, u32Ret, &len, aBuf);
		printf("Open PCI Card failed: %s\n", aBuf);
#endif
		CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_OPEN_DEV_FAILURE);
		return -4;
	}

	global_sm1_key.dev_type = vendor_fm;
	return 0;
}

int SM1_set_encrypt_key(const unsigned char *userKey, const int bits,
	SM1_KEY *key)
{
	static int key_inited = 0;

	/* init struct SM1_KEY if necessary */
	if(!key_inited) {
		int vendor = 0, subtype = 0;

		if (get_smcard_info(&vendor, &subtype)) {
			CRYPTOerr(CRYPTO_F_INT_SM_CARD,CRYPTO_R_GET_SMCARD_TYPE);
			return -1;
		}
		switch(vendor)
		{
		case vendor_fm:
			if(Load_and_Open_fmdev(subtype))
				return -1;
			break;
		default:
			break;
		}

		key_inited = 1;
	}

	memset(key, 0, sizeof(*key));
	/* restore key */
	key->key_size = bits>>3;
	memcpy(key->key_data, userKey, key->key_size);

	return 0;
}

int SM1_set_decrypt_key(const unsigned char *userKey, const int bits,
	SM1_KEY *key)
{
	return SM1_set_encrypt_key(userKey, bits, key);
}

void SM1_cbc_encrypt(const unsigned char *in, unsigned char *out,
    size_t length, const SM1_KEY *key,
    unsigned char *ivec, const int enc)
{
	FM_U32 u32Ret = FME_OK;
	FM_U32 outLen = 0;
	u_int8_t *outData = NULL;

	if(!global_sm1_key.dev_handle) {
#ifdef SM1_TEST
		printf("handle is null\n");
#endif
		goto end;
	}

	outData = OPENSSL_malloc(length*sizeof(u_int8_t));
	if(!outData)
	{
		printf("%s() malloc failed\n", __func__);
		goto end;
	}

	if(enc) {
		if(NULL == global_sm1_key.enc_func)
		{
#ifdef SM1_TEST
			printf("Encrypt function point is null\n");
#endif
			goto end;
		}

		u32Ret = ((fm_Encrypt *)(global_sm1_key.enc_func))(global_sm1_key.dev_handle,
								FM_HKEY_FROM_HOST,  FM_ALG_SM1, FM_ALGMODE_CBC,
								(FM_U8 *)in, length, outData, &outLen,
								(FM_U8 *)key->key_data, key->key_size,
								ivec, SM1_IV_SIZE);
		if (FME_OK != u32Ret)
		{
#ifdef SM1_TEST
			FM_U32 len = 0;
			FM_S8 aBuf[128];
			((fm_ErrInfo *)(global_sm1_key.get_errinfo))(1, u32Ret, &len, aBuf);
			printf("FM_CPC_Encrypt failed [%s]\n", aBuf);
#endif
			goto end;
		}
	}
	else {
		if(NULL == global_sm1_key.dec_func)
		{
#ifdef SM1_TEST
			printf("Decrypt function point is null\n");
#endif
			goto end;
		}
		u32Ret = ((fm_Decrypt *)(global_sm1_key.dec_func))(global_sm1_key.dev_handle,
								FM_HKEY_FROM_HOST,  FM_ALG_SM1, FM_ALGMODE_CBC,
								(FM_U8 *)in, length, outData, &outLen,
								(FM_U8 *)key->key_data, key->key_size,
								ivec, SM1_IV_SIZE);
		if (FME_OK != u32Ret)
		{
#ifdef SM1_TEST
			FM_U32 len = 0;
			FM_S8 aBuf[128];
			((fm_ErrInfo *)(global_sm1_key.get_errinfo))(1, u32Ret, &len, aBuf);
			printf("FM_CPC_Decrypt failed [%s]\n", aBuf);
#endif
			goto end;
		}
	}
	memcpy(out, outData, outLen);

end:
	if(outData)
		OPENSSL_free(outData);
	return;
}
