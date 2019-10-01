#include "blockchain.h"
#include <string.h>

void blockchain_append(Blockchain *this, void *data, uint64_t sz,
                       uint8_t *prevh);
void blockchain_verify_block(Blockchain *this);
void blockchain_verify_chain(Blockchain *this);
Block *blockchain_prev(Blockchain *this);

Block *blockchain_prev(Blockchain *this) {

}

void blockchain_append(Blockchain *this, void *data, uint64_t sz,
                       uint8_t *prevh) {

  Block *block = malloc(sizeof(struct Block));
  block->node = malloc(sizeof(struct Node));
  node_init(block->node, data, sz);

  memcpy(block->prevh, prevh, SHA256_DIGEST_LENGTH);

  block->timestamp = time(NULL);
  block->index = this->index;
  this->index++;

  this->ll->insert_front(this->ll, data, sz);

  // hash the block

}

void blockchain_init(Blockchain *blockchain) {
  char *msg = "this is the initial block message";
  uint64_t msg_sz = strlen(msg)+1; // include null

  uint8_t *prevh = malloc(SHA256_DIGEST_LENGTH);

  blockchain->index = 0;
  blockchain->ll = malloc(sizeof(struct LinkedList));

  linkedlist_init(blockchain->ll);
  blockchain_append(blockchain, msg, msg_sz, prevh);

  free(prevh);

  // set function pointers
  blockchain->append = &blockchain_append;
  blockchain->prev = &blockchain_prev;
}

void blockchain_destroy(Blockchain *blockchain) {
  // blockchain needs an iterator to remove all the block headers
  // before the linked list is destroyed. this means that block_chain
  // destroy will take 2n operations, unless the blockchain and linkedlist
  // data structures are merged

  linkedlist_destroy(blockchain->ll);
  free(blockchain->ll);
}
