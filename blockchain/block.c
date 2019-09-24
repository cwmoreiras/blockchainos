#include "block.h"
#include "crypto.h"
#include <string.h>
#include <time.h>

void block_generator(block *b) {
  int index;
  unsigned char prev_hash[SHA256_BLOCK_SZ];
  time_t timestamp;
  unsigned char data[BLOCK_DATA_SZ];
  unsigned char hash[SHA256_BLOCK_SZ];

}

void block_create(block *b) {
  int index;
  unsigned char prev_hash[SHA256_BLOCK_SZ];
  int timestamp;
  int data;
  unsigned char hash[SHA256_BLOCK_SZ];

  SHA256_CTX sha;

  size_t testlen = 5;
  unsigned char test[5] = {'t', 'e', 's', 't', '\0'};

  size_t hashlen = SHA256_BLOCK_SZ;
  size_t datalen = BLOCK_DATA_SZ;

  block_generator(b);


  sha256_init(&sha);
  sha256_update(&sha, test, testlen);
  sha256_final(&sha, hash);

  b->index = index;
  memcpy(b->prev_hash, hash, hashlen);
  b->timestamp = timestamp;
  memcpy(b->data, data, datalen);
  memcpy(b->hash, hash, hashlen);
}
