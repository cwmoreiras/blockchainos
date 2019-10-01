#ifndef BLOCK_H
#define BLOCK_H

#include "node.h"
#include "linkedlist.h"

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

// subclass of Node, inherites prev, next, data
typedef struct Block Block;
struct Block{
  Node *node;
  uint8_t prevh[SHA256_DIGEST_LENGTH]; // previous blocks hash
  uint64_t timestamp;
  uint64_t index;
  uint8_t thish[SHA256_DIGEST_LENGTH]; // this blocks hash
};


typedef struct Blockchain Blockchain;
struct Blockchain {
  LinkedList *ll;
  uint64_t index;
  void (*append)(Blockchain *this, void *data, uint64_t sz, uint8_t *prevh);
  Block *(*prev)(Blockchain *this);
};
void blockchain_init(Blockchain *blockchain);
void blockchain_destroy(Blockchain *blockchain);



#endif
