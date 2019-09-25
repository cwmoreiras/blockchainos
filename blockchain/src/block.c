#include "block.h"
#include "crypto.h"
#include "util.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void block_print(const block *b)
{
  int i;
  printf("index:           0x");
  for (i = BLOCK_POS_INDEX; i < BLOCK_POS_TIMESTAMP; i++)
    printf("%02x", b->buf[i]);
  printf("\n");

  printf("timestamp:       0x");
  for (; i < BLOCK_POS_DATA; i++)
    printf("%02x", b->buf[i]);
  printf("\n");

  printf("data:            0x");
  for (; i < BLOCK_POS_PREVHASH; i++)
    printf("%02x", b->buf[i]);
  printf("\n");

  printf("prev_hash:       0x");
  for (; i < BLOCK_POS_HASH; i++)
    printf("%02x", b->buf[i]);
  printf("\n");

  printf("hash:            0x");
  for (; i < BLOCK_SZ; i++)
    printf("%02x", b->buf[i]);
  printf("\n");

  printf("block:           0x");
  for (i = 0; i < BLOCK_SZ; i++)
    printf("%02x", b->buf[i]);
  printf("\n");
}

void block_genesis(block *b)
// -----------------------------------------------------------------------------
// func: generate the initial block for the chain
// args: b - a storage container for the block data
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  unsigned char *msg = (unsigned char *)"hello world. this is a message that \
    can change but that i w";
  size_t msg_sz = strlen((char*)msg)+1; // include the null
  time_t ts = time(NULL); // get timestamp

  unsigned char hash[BLOCK_NB_HASH];

  memset(b->buf, 0, BLOCK_SZ); // initialize the block to all zeroes
  memcpy(&b->buf[BLOCK_POS_TIMESTAMP], &ts, BLOCK_NB_TIMESTAMP); // fill ts
  memcpy(&b->buf[BLOCK_POS_DATA], msg, msg_sz); // fill the data

  block_calc_hash(b, hash); // hash this block

  // copy the hash into the block
  memcpy(&b->buf[BLOCK_POS_HASH], hash, BLOCK_NB_HASH);
}

/*
void block_create(const block *old_block, block *new_block,
                  const unsigned char data[])
// -----------------------------------------------------------------------------
// func: create a new block from the data and previous block info
// args: old_block - previous block in the chain
//       new_block - the newly created block
//       data - the data to store in the new block
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{

}
*/

void block_calc_hash(const block *b, unsigned char hash[])
// -----------------------------------------------------------------------------
// func: calculate the hash of the passed block
// args: b - the block for which a hash is desired
//       hash - the block hash
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  int sz = 8;
  unsigned char buf[sz];
  int i;
  for (i = 0; i < sz; i++)
    buf[i] = 0x61;

  SHA256_CTX sha[SHA256_DIGEST_SZ];

  sha256_init(sha);
  sha256_update(sha, (unsigned char *) buf, sz);
  //sha256_update(sha, b->buf, BLOCK_SZ-BLOCK_NB_HASH);
  sha256_final(sha, hash);

}
