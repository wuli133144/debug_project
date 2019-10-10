#ifndef SM3_SOFT_H
#define SM3_SOFT_H

typedef struct
{
	unsigned int total[2];     /*!< number of bytes processed  */
	unsigned int state[8];     /*!< intermediate digest state  */
	unsigned char buffer[64];   /*!< data block being processed */
} sm3_context;

void sm3_soft_init( sm3_context *ctx );
void sm3_soft_update( sm3_context *ctx, const unsigned char *input, int ilen );
void sm3_soft_final( sm3_context *ctx, unsigned char* output);

#endif
