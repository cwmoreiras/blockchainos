#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"
#include "util.h"

int main() {
  block *b;

  b = malloc(sizeof(block));
  block_genesis(b);

  block_print(b);

  // dont write out the hash for the test
  // we want to hash it without that segment
  util_buf_write_raw(b->buf, BLOCK_SZ-BLOCK_NB_HASH, "mainout");

  free(b);
}
