#ifndef BLOCK_H
#define BLOCK_H

#include "crypto.h"
#include <time.h>

// NB prefix -> number of bytes
#define BLOCK_NB_INDEX     8
#define BLOCK_NB_TIMESTAMP 8
#define BLOCK_NB_DATA      64
#define BLOCK_NB_PREVHASH  32
#define BLOCK_NB_HASH      32
#define BLOCK_NB_TOTAL     144

typedef struct {
  unsigned char buf[BLOCK_NB_TOTAL];
} block;

void block_genesis(block *);
void block_create(const block *, block *, const unsigned char[]);
void block_calc_hash(const block *, unsigned char[]);
void block_print(const block *);

#endif
