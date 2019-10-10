/* crypto/sm2/sm2_err.c */

#include <stdio.h>
#include <openssl/err.h>
#include <openssl/sm2.h>

/* BEGIN ERROR CODES */
#ifndef OPENSSL_NO_ERR

#define ERR_FUNC(func) ERR_PACK(ERR_LIB_SM2,func,0)
#define ERR_REASON(reason) ERR_PACK(ERR_LIB_SM2,0,reason)

static ERR_STRING_DATA SM2_str_functs[]=
	{
{ERR_FUNC(SM2_F_SM2_CHECK),	"SM2_CHECK"},
{ERR_FUNC(SM2_F_SM2_DATA_NEW_METHOD),	"SM2_DATA_NEW_METHOD"},
{ERR_FUNC(SM2_F_SM2_DO_SIGN),	"SM2_do_sign"},
{ERR_FUNC(SM2_F_SM2_DO_VERIFY),	"SM2_do_verify"},
{ERR_FUNC(SM2_F_SM2_SIGN_SETUP),	"SM2_sign_setup"},
{0,NULL}
	};

static ERR_STRING_DATA SM2_str_reasons[]=
	{
{ERR_REASON(SM2_R_BAD_SIGNATURE)       ,"bad signature"},
{ERR_REASON(SM2_R_DATA_TOO_LARGE_FOR_KEY_SIZE),"data too large for key size"},
{ERR_REASON(SM2_R_DECRYPT_FAILED)          ,"decrypt data failed"},
{ERR_REASON(SM2_R_MISSING_PARAMETERS)  ,"missing parameters"},
{ERR_REASON(SM2_R_NEED_NEW_SETUP_VALUES),"need new setup values"},
{ERR_REASON(SM2_R_NON_FIPS_METHOD)     ,"non fips method"},
{ERR_REASON(SM2_R_RANDOM_NUMBER_GENERATION_FAILED),"random number generation failed"},
{ERR_REASON(SM2_R_SIGNATURE_MALLOC_FAILED),"signature malloc failed"},
{ERR_REASON(SM2_R_UNSUPPORT_COMPRESSED),"unsupport to decrypt compressed data"},
{0,NULL}
	};

#endif

void ERR_load_SM2_strings(void)
{
#ifndef OPENSSL_NO_ERR

	if (ERR_func_error_string(SM2_str_functs[0].error) == NULL)
	{
		ERR_load_strings(0,SM2_str_functs);
		ERR_load_strings(0,SM2_str_reasons);
	}
#endif
}
