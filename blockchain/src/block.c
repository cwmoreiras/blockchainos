#include "block.h"
#include "util.h"

#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

void block_print_segment(const block *b, const int start, const int end) {
  int i;

  switch(start) {
    case BLOCK_POS_INDEX:     printf("%-10s-> 0x", "index"); break;
    case BLOCK_POS_TIMESTAMP: printf("%-10s-> 0x", "timestamp"); break;
    case BLOCK_POS_DATA: ;    printf("%-10s-> 0x", "data"); break;
    case BLOCK_POS_PREVHASH:  printf("%-10s-> 0x", "prev_hash"); break;
    case BLOCK_POS_HASH:      printf("%-10s-> 0x", "hash"); break;
    default:                  printf("%-10d-> 0x", start); break;
  }

  for (i = start; i < end; i++) {
    printf("%02x", b->buf[i]);
  }
  printf("\n");

}


void block_print(const block *b)
{
  block_print_segment(b, BLOCK_POS_INDEX, BLOCK_POS_TIMESTAMP);
  block_print_segment(b, BLOCK_POS_TIMESTAMP, BLOCK_POS_DATA);
  block_print_segment(b, BLOCK_POS_DATA, BLOCK_POS_PREVHASH);
  block_print_segment(b, BLOCK_POS_PREVHASH, BLOCK_POS_HASH);
  block_print_segment(b, BLOCK_POS_HASH, BLOCK_SZ);
}

void block_genesis(block *b)
// -----------------------------------------------------------------------------
// func: generate the initial block for the chain
// args: b - a storage container for the block data
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  uint8_t *msg = (uint8_t *)"hello world. this is a message that can change but that i ";
  size_t msg_sz = strlen((char*)msg)+1; // include the null
  time_t ts = time(NULL); // get timestamp

  memset(b->buf, 0, BLOCK_SZ); // initialize the block to all zeroes
  memcpy(&b->buf[BLOCK_POS_TIMESTAMP], &ts, BLOCK_NB_TIMESTAMP); // fill ts
  memcpy(&b->buf[BLOCK_POS_DATA], msg, msg_sz); // fill the data

  block_calc_hash(b); // hash this block, and store the hash
}

/*
void block_create(const block *old_block, block *new_block,
                  const uint8_t data[])
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

void block_calc_hash(block *b)
// -----------------------------------------------------------------------------
// func: calculate the hash of the passed block
// args: b - the block for which a hash is desired
//       hash - the block hash
// retn: none
// auth: cwmoreiras
// -----------------------------------------------------------------------------
{
  uint8_t hash[BLOCK_NB_HASH];
  SHA256_CTX sha[SHA256_DIGEST_LENGTH];

  SHA256_Init(sha);

  // hash the whole block except the part that contains where the hash going
  // to go
  SHA256_Update(sha, b->buf, BLOCK_SZ-BLOCK_NB_HASH);
  SHA256_Final(hash, sha);

  // copy the hash into the block
  memcpy(&b->buf[BLOCK_POS_HASH], hash, BLOCK_NB_HASH);
}
