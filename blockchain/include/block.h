#ifndef BLOCK_H
#define BLOCK_H

#include "crypto.h"
#include <time.h>

// NB prefix -> number of bytes
#define NB_INDEX     8
#define NB_TIMESTAMP 8
#define NB_DATA      64
#define NB_PREVHASH  64
#define NB_HASH      64
#define NB_TOTAL     208

typedef struct block {
  int index;
  unsigned char prev_hash[SHA256_DIGEST_SZ];
  time_t timestamp;
  unsigned char data[SHA256_BLOCK_SZ];
  unsigned char hash[SHA256_DIGEST_SZ];
} block;

void block_genesis(block *);
void block_create(const block *, block *, const unsigned char[]);
void block_calc_hash(const block *, unsigned char[]);

#endif
