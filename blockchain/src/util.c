#include "util.h"
#include "block.h"
#include "crypto.h"
#include <stdio.h>

void print_block(block *b)
{
  unsigned char buf[2*SHA256_DIGEST_SZ+1];

  printf("********* Block Print *********\n");
  printf("index: %d\n", b->index);
  printf("prev_hash: %02hhn\n", b->prev_hash);
  printf("timestamp: %lu\n", b->timestamp);
  printf("data: %s\n", b->data);
  printf("hash: %s\n", b->hash);
}
