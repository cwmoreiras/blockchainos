#include "blockchain.h"
#include <string.h>

#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

void blockchain_verify_block(Blockchain *this);
void blockchain_verify_chain(Blockchain *this);
void blockchain_root(Blockchain *this);
void blockchain_append(Blockchain *this,
                       uint8_t *record,
                       uint64_t record_sz);

void block_frame(Block *this, uint8_t *buf) {
  memcpy(&buf[PREVHASH_POS], this->prevhash, SHA256_DIGEST_LENGTH);
  memcpy(&buf[CURRHASH_POS], this->hash, SHA256_DIGEST_LENGTH);
  memcpy(&buf[INDEX_POS], &this->index, sizeof(uint64_t));
  memcpy(&buf[TS_POS], &this->timestamp, sizeof(uint64_t));
  memcpy(&buf[RECORD_SZ_POS], &this->record_sz, sizeof(uint64_t));
  memcpy(&buf[RECORD_POS], this->record, this->record_sz);
}

void block_hash(Block *this, uint8_t *hash) {
  uint8_t *buf[BLOCK_HEADER_SZ + this->record_sz];

  memcpy(&buf[PREVHASH_POS], this->prevhash, SHA256_DIGEST_LENGTH);
  memcpy(&buf[CURRHASH_POS], this->hash, SHA256_DIGEST_LENGTH);
  memcpy(&buf[INDEX_POS], &this->index, sizeof(uint64_t));
  memcpy(&buf[TS_POS], &this->timestamp, sizeof(uint64_t));
  memcpy(&buf[RECORD_SZ_POS], &this->record_sz, sizeof(uint64_t));
  memcpy(&buf[RECORD_POS], this->record, this->record_sz); // this->record allocation

  SHA256_CTX sha;
  SHA256_Init(&sha);
  SHA256_Update(&sha, buf, BLOCK_HEADER_SZ + this->record_sz);
  SHA256_Final(hash, &sha);

}

void blockchain_root(Blockchain *this) {
  Block block;
  uint8_t *record = (uint8_t *)"this is the first block";
  uint64_t record_sz = strlen((char *)record)+1;
  uint64_t blocksize = BLOCK_HEADER_SZ + record_sz;
  uint8_t buf[blocksize];
  uint8_t hash[SHA256_DIGEST_LENGTH];

  block.record_sz = record_sz;
  block.record = malloc(block.record_sz);
  memcpy(block.record, record, block.record_sz);

  block.index = 0;
  block.timestamp = time(NULL);

  memset(block.prevhash, 0, SHA256_DIGEST_LENGTH);
  memset(block.hash, 0, SHA256_DIGEST_LENGTH);

  block_hash(&block, hash);
  memcpy(&block.hash, hash, SHA256_DIGEST_LENGTH);
  block_frame(&block, buf);

  this->insert_front(this, buf, blocksize); // the
  free(block.record);

}

void blockchain_append(Blockchain *this,
                       uint8_t *record,
                       uint64_t record_sz) {
  Block block;
  uint64_t blocksize = BLOCK_HEADER_SZ + record_sz;
  uint8_t buf[blocksize];
  uint8_t hash[SHA256_DIGEST_LENGTH];
  Block *prevblock = (Block *)this->peek_front(this);

  block.record_sz = record_sz;
  block.record = malloc(block.record_sz);
  memcpy(block.record, record, block.record_sz);

  block.timestamp = time(NULL);
  block.index = prevblock->index+1;

  memcpy(block.prevhash, prevblock->hash, SHA256_DIGEST_LENGTH);
  memset(block.hash, 0, SHA256_DIGEST_LENGTH);

  block_hash(&block, hash);
  memcpy(&block.hash, hash, SHA256_DIGEST_LENGTH);
  block_frame(&block, buf);

  printf("%s", block.record);
  this->insert_front(this, buf, blocksize);
  free(block.record);

}

void blockchain_init(Blockchain *blockchain) {
  linkedlist_init(blockchain);
  blockchain->append = &blockchain_append;
  blockchain_root(blockchain);

}

void blockchain_destroy(Blockchain *blockchain) {
  linkedlist_destroy(blockchain);
}
