#include "block.h"
#include "crypto.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

void block_genesis(block *b)
// -----------------------------------------------------------------------------
// func: generate the initial block for the chain
// args: b - a storage container for the block data
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  unsigned char *data = (unsigned char *)"hello world";
  size_t datalen = strlen((char*)data) + 1; // hash the null character also

  // 1. first index is 0
  // 2. previous hash field for initial block is 0
  // 3. timestamp will be correct
  // 4. data for the first block is "hello world"
  // 5. hash the block
  b->index = 0;
  memset(b->prev_hash, 0, SHA256_DIGEST_SZ);
  b->timestamp = time(NULL);
  memcpy(b->data, data, datalen);
  block_calc_hash(b, b->hash);
}

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
  // 1. increment the index of the block
  // 2. copy the previous block's hash into this block
  // 3. set the timestamp
  // 4. calculate this block's hash
  new_block->index = old_block->index+1;
  memcpy(new_block->prev_hash, old_block->hash, SHA256_DIGEST_SZ);
  new_block->timestamp = time(NULL);
  memcpy(new_block->data, data, SHA256_BLOCK_SZ);
  block_calc_hash(new_block, new_block->hash);
}

void block_calc_hash(const block *b, unsigned char hash[])
// -----------------------------------------------------------------------------
// func: calculate the hash of the passed block
// args: b - the block for which a hash is desired
//       hash - the block hash
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  SHA256_CTX sha;
  int i;
  int sum;

  unsigned char buf[SHA256_STR_SZ] = "";

  sum = b->index; // add the index
  sum += b->timestamp; // add the timestamp
  // add the previous hash, which is a number
  // add the data


  snprintf((char *)buf, SHA256_STR_SZ, "%02x", sum);

  sha256_init(&sha);
  sha256_update(&sha, buf , SHA256_DIGEST_SZ);
  sha256_final(&sha, hash);

  print_block(b);
}
