/* crypto/sm3/sm3test.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../e_os.h"

#ifdef OPENSSL_NO_SM3
int main(int argc, char *argv[])
{
    printf("No SM3 support\n");
    return(0);
}
#else
#include <openssl/evp.h>
#include <openssl/sm3.h>

static char *test[]={
	"abc",
	"abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd",
	NULL,
	};

static char *ret[]={
	"66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0",
	"debe9ff92275b8a138604889c18e5a4d6fdb70e5387e5765293dcba39c0c5732",
	};

static char *pt(unsigned char *md);
int main(int argc, char *argv[])
	{
	int i,err=0;
	char **P,**R;
	char *p;
	unsigned char md[SM3_DIGEST_LENGTH];

	P=test;
	R=ret;
	i=1;
	while (*P != NULL)
		{
		EVP_Digest(&(P[0][0]),strlen((char *)*P),md,NULL,EVP_sm3(), NULL);
		p=pt(md);
		if (strcmp(p,(char *)*R) != 0)
			{
			printf("error calculating SM3 on '%s'\n",*P);
			printf("got %s instead of %s\n",p,*R);
			err++;
			}
		else
			printf("test %d ok\n",i);
		i++;
		R++;
		P++;
		}

#ifdef OPENSSL_SYS_NETWARE
    if (err) printf("ERROR: %d\n", err);
#endif
	EXIT(err);
	return(0);
	}

static char *pt(unsigned char *md)
	{
	int i;
	static char buf[80];

	for (i=0; i<SM3_DIGEST_LENGTH; i++)
		sprintf(&(buf[i*2]),"%02x",md[i]);
	return(buf);
	}
#endif
