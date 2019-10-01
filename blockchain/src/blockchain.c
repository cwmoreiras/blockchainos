#include "blockchain.h"

void blockchain_genesis(Blockchain *this);
void blockchain_append(Blockchain *this);
void blockchain_verify_block(Blockchain *this);
void blockchain_verify_chain(Blockchain *this);

void blockchain_init(Blockchain *this) {
  //bc->genesis = &blockchain_genesis;
  //bc->append = &blockchain_append;
  //bc->verify_block = &blockchain_verify_block;
  //bc->verify_chain = &blockchain_verify_chain;
}

void blockchain_destroy(Blockchain *this) {

}


// have the chain handle  everything except the data, which
// is given by the user
void block_init(Block *this, void *data, uint64_t sz) {

}
void block_destroy(Block *this) {

}
