#include "block.h"
#include "crypto.h"
#include <string.h>
#include <time.h>

void block_genesis(block *b) {
  b->index = 0;
  //b->prev_hash = (unsigned char *) bzero;
  b->timestamp = time(NULL);
  //b->data = (unsigned char *) "firstblock";
  //b-> hash =

}

void block_create(block *old_block, block *new_block) {
  int index;
  unsigned char prev_hash[SHA256_BLOCK_SZ];
  int timestamp;
  unsigned char data[BLOCK_DATA_SZ];
  unsigned char hash[SHA256_BLOCK_SZ];

  SHA256_CTX sha;

  size_t testlen = 5;
  unsigned char test[5] = {'t', 'e', 's', 't', '\0'};

  size_t hashlen = SHA256_BLOCK_SZ;
  size_t datalen = BLOCK_DATA_SZ;

  sha256_init(&sha);
  sha256_update(&sha, test, testlen);
  sha256_final(&sha, hash);

  new_block->index = old_block->index+1;
  memcpy(new_block->prev_hash, hash, hashlen);
  new_block->timestamp = timestamp;
  memcpy(new_block->data, data, datalen);
  memcpy(new_block->hash, hash, hashlen);
}
