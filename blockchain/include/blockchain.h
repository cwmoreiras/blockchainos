#ifndef BLOCK_H
#define BLOCK_H

#include "node.h"
#include "linkedlist.h"

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#define BLOCK_HEADER_SZ 88

#define HASH_SZ         SHA256_DIGEST_LENGTH
#define WORD_SZ         8 // 8 byte words for high integer counters, etc
                          // probably unecessary?

#define PREVHASH_POS    0
#define CURRHASH_POS    32
#define INDEX_POS       64
#define TS_POS          72
#define RECORD_SZ_POS   80
#define RECORD_POS      88

typedef struct Block Block;
struct Block {
  uint8_t prevhash[HASH_SZ];
  uint8_t hash[HASH_SZ];
  uint64_t index;
  uint64_t timestamp;
  uint64_t record_sz;
  uint8_t *record;
};

// TODO encapsulate this function?
void blockframe_decode(uint8_t *this,
                       uint8_t *prevhash, uint8_t *hash,
                       uint64_t *index, uint64_t *timestamp,
                       uint64_t *record_sz, uint8_t *record);

typedef struct LinkedList Blockchain; // blockchain is a linked list
void blockchain_init(Blockchain *blockchain);
void blockchain_destroy(Blockchain *blockchain);



#endif
