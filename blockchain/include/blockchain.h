#ifndef BLOCK_H
#define BLOCK_H

#include "node.h"
#include "linkedlist.h"

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

// subclass of Node, inherites prev, next, data
typedef struct {
  Node node;
  uint8_t prevh[SHA256_DIGEST_LENGTH]; // previous blocks hash
  uint64_t index;
  uint64_t timestamp;
  uint8_t thish[SHA256_DIGEST_LENGTH]; // this blocks hash

} Block;
void block_init(Block *this, void *data, uint64_t sz);
void block_destroy(Block *this);

typedef struct {
  LinkedList ll;
  //void (*genesis)(Blockchain *this);
  //void (*append)(Blockchain *this, void *data, uint64_t sz);
  //void (*verify_block)(Blockchain *this);
  //void (*verify_chain)(Blockchain *this);
} Blockchain;
void blockchain_init(Blockchain *this);
void blockchain_destroy(Blockchain *this);

#endif
