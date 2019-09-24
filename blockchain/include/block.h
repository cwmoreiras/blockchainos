#ifndef BLOCK_H
#define BLOCK_H

#include "crypto.h"
#include <time.h>

#define BLOCK_DATA_SZ 4

typedef struct block {
  int index;
  unsigned char prev_hash[SHA256_BLOCK_SZ];
  time_t timestamp;
  unsigned char data[BLOCK_DATA_SZ];
  unsigned char hash[SHA256_BLOCK_SZ];
} block;

void block_genesis(block *);
void block_create(block *, block *);

#endif
