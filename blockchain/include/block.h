#ifndef BLOCK_H
#define BLOCK_H

#include "util.h"

#include <stdint.h>
#include <time.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

// NB prefix -> number of bytes
#define BLOCK_NB_INDEX     8
#define BLOCK_NB_TIMESTAMP 8
#define BLOCK_NB_DATA      64
#define BLOCK_NB_PREVHASH  32
#define BLOCK_NB_HASH      32

#define BLOCK_POS_INDEX     0
#define BLOCK_POS_TIMESTAMP 8
#define BLOCK_POS_DATA      16
#define BLOCK_POS_PREVHASH  80
#define BLOCK_POS_HASH      112

#define BLOCK_SZ            144

typedef struct {
  uint8_t prev_hash [SHA256_DIGEST_LENGTH];
  uint64_t index;
  uint64_t timestamp;
  DynArray *data;
  uint8_t hash [SHA256_DIGEST_LENGTH];
} block;

void block_test_hash();
void block_genesis(block *);
void block_create(const block *, block *, const uint8_t[]);
void block_calc_hash(block *);
void block_print_segment(const block *b, const int start, const int end);
void block_print(const block *b);
void block_write_raw();

#endif
