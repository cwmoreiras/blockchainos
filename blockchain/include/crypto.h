// -----------------------------------------------------------------------------
// Filename:   crypto.h
// Author:     Brad Conte (brad AT bradconte.com)
// Modified:   Carlos Moreiras
// Copyright:
// Disclaimer: This code is presented "as is" without any guarantees.
// Details:    Defines the API for the corresponding SHA256 implementation.
// -----------------------------------------------------------------------------

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdint.h>

#define SHA256_DIGEST_SZ 32 // SHA256 outputs a 32 byte digest
#define SHA256_BLOCK_SZ 64  // SHA256 operates on 64 byte chunks

typedef struct {
	uint8_t data[SHA256_BLOCK_SZ];
	uint32_t datalen;
	uint64_t bitlen;
	uint32_t state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

#endif
