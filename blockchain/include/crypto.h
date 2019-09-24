/*********************************************************************
* Filename:   crypto.h
* Author:     Brad Conte (brad AT bradconte.com)
* Modified:   Carlos Moreiras
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA256 implementation.
*********************************************************************/

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SZ 32            // SHA256 outputs a 32 byte digest

/**************************** DATA TYPES ****************************/
typedef struct {
	unsigned char data[64];
	unsigned int datalen;
	unsigned long long bitlen;
	unsigned int state[8];
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len);
void sha256_final(SHA256_CTX *ctx, unsigned char hash[]);

#endif
