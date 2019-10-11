/*
blockchain.c: method definitions for blockchain structure
Copyright (C) 2019 
maintainer: Carlos WM
email: cwmoreiras@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "blockchain.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//----------------------//
// "PRIVATE" PROTOTYPES //
//----------------------//

// LinkedList "inherited" functions
// int blockchain_delete_front(Blockchain *this);
void blockchain_insert_front(Blockchain *this,
               uint8_t *record,
               uint64_t record_sz);
void *blockchain_peek_front(Blockchain *this);

// Blockchain functions
int blockchain_verify_block(Block *new_block, Block *old_block);
void blockchain_verify_chain(Blockchain *this);
void blockchain_root(Blockchain *this);
// Block functions
void block_hash(Block *this, uint8_t *hash);
void block_frame(Block *this, uint8_t *buf);
// BlockFrame functions
void blockframe_decode(uint8_t *blockframe, Block *block);
void blockframe_print(uint8_t *this);

//-----------------//
// IMPLEMENTATIONS //
//-----------------//

int blockchain_verify_block(Block *block, Block *prev_block) {
  uint64_t blocksize = BLOCK_HEADER_SZ + block->record_sz;
  uint8_t buf[blocksize];
  uint8_t hash[HASH_SZ];

  util_buf_hash(buf, blocksize, hash);

  if (prev_block->index + 1 != block->index+1)
    return 0;
  else if (prev_block->hash != block->prevhash)
    return 0;
  else if (!strncmp((char *)hash, (char *)block->hash,  HASH_SZ))
    return 0;

  return 1;

}

void blockchain_init(Blockchain *this)
// -----------------------------------------------------------------------------
// Func: Initialize a new blockchain
// Args: this - a pointer to the new chain object
// Retn: None
// -----------------------------------------------------------------------------
{
  this->ll = malloc(sizeof(LinkedList));
  linkedlist_init(this->ll); // blockchain is just a fancy linkedlist
  
  // Override/map methods
  this->insert_front = &blockchain_insert_front;
  // this->delete_front = &blockchain_delete_front;
  this->peek_front = &blockchain_peek_front;
  this->verify_block = &blockchain_verify_block;


  blockchain_root(this); // build and attach the root block
}

void *blockchain_peek_front(Blockchain *this)
// -----------------------------------------------------------------------------
// Func: Gets first framed block at front of chain
// Args: this - a pointer to the blockchain
// Retn: Pointer to framed block
// -----------------------------------------------------------------------------

{
  return this->ll->peek_front(this->ll);
}

void blockchain_insert_front(Blockchain *this,
                       uint8_t *record,
                       uint64_t record_sz)
// -----------------------------------------------------------------------------
// Func: Append a record to the blockchain
// Args: this - a pointer to the blockchain
//       record - the record that we'd like to append
//       record_sz - the size of the record
// Retn: None
// -----------------------------------------------------------------------------
{
  Block block;
  Block prev_block;

  uint64_t blocksize = BLOCK_HEADER_SZ + record_sz;
  uint8_t buf[blocksize];
  uint8_t hash[HASH_SZ];

  // get a pointer to the previous block
  uint8_t *prev_blockframe = (uint8_t *)this->peek_front(this);
  prev_block.record = malloc(prev_blockframe[RECORD_SZ_POS]);
  blockframe_decode(prev_blockframe, &prev_block);

  block.record_sz = record_sz;
  block.record = malloc(block.record_sz);
  memcpy(block.record, record, block.record_sz); // copy record into block

  block.timestamp = time(NULL);
  block.index = prev_block.index+1; // increment index

  memcpy(block.prevhash, prev_block.hash, HASH_SZ); // copy the prev blocks hash
  memset(block.hash, 0, HASH_SZ); // set the hash field to 0

  block_hash(&block, hash); // hash the block
  memcpy(&block.hash, hash, HASH_SZ); // copy the hash into the hash field
  block_frame(&block, buf); // frame for storage
 
  this->ll->insert_front(this->ll, buf, blocksize); // append to the chain
  free(block.record); // free the local copy

}

// int blockchain_delete_front(Blockchain *this) 
// {
//   return -1; // you can't delete blocks how you would in a linkedlist
// }

void blockchain_root(Blockchain *this)
// -----------------------------------------------------------------------------
// Func: Construct the blockchain's root block (must be hardcoded)
// Args: this - the blockchain for which we need a root
// Retn: None
// -----------------------------------------------------------------------------
{
  Block block;
  uint8_t *record = (uint8_t *)"this is the first block"; // message can change
  uint64_t record_sz = strlen((char *)record)+1; // count the null character
  uint64_t blocksize = BLOCK_HEADER_SZ + record_sz;
  uint8_t buf[blocksize];
  uint8_t hash[HASH_SZ];

  block.record_sz = record_sz;
  block.record = malloc(block.record_sz);
  memcpy(block.record, record, block.record_sz);

  block.index = 0; // its the root block
  block.timestamp = time(NULL);

  memset(block.prevhash, 0, HASH_SZ);
  memset(block.hash, 0, HASH_SZ);

  // this will hash the whole block, with 0's in the prevhash and hash fields
  block_hash(&block, hash); 
  memcpy(&block.hash, hash, HASH_SZ); // fill the hash field with the result 
  block_frame(&block, buf); // frame it to remove 0-padding 

  this->ll->insert_front(this->ll, buf, blocksize); // add the framed block to the chain
  free(block.record); // free the local copy

}

void blockchain_destroy(Blockchain *this)
// -----------------------------------------------------------------------------
// Func: Destroy a blockchain object
// Args: blockchain - point to the blockchain object we'd like to destroy
// Retn: None
// -----------------------------------------------------------------------------
{
  linkedlist_destroy(this->ll); // just need to destroy the list
  free(this->ll);
}


void blockframe_print(uint8_t *blockframe) 
// -----------------------------------------------------------------------------
// Func: Decodes the framed block and prints its contents
// Args: this - pointer to framed block
// Retn: None
// -----------------------------------------------------------------------------
{
  Block block;

  block.record = malloc(blockframe[RECORD_SZ_POS]);
  blockframe_decode(blockframe, &block);

  printf("-------------------------------------------------------------------------\n");
  util_buf_print_hex(block.prevhash, HASH_SZ, "phash", 1);
  util_buf_print_hex(block.hash, HASH_SZ, "hash ", 1);

  printf("index: %lu\n", block.index);
  printf("tstmp: %lu\n", block.timestamp);
  printf("recsz: %lu\n", block.record_sz);

  util_buf_print_hex(block.record, block.record_sz, "recrd", 1);
  free(block.record);
}

void blockframe_decode(uint8_t *blockframe, Block *block)
// -----------------------------------------------------------------------------
// Func: Inverse of block_frame function. Takes a block frame and extracts its 
//       contents to facilitate certain processing needs
// Args: this - a pointer to the block frame
//       prevhash - pointer to the previous hash
//       hash - a pointer to this block's hash
//       index - pointer to the index of this block
//       timestamp - pointer to this block's original timestamp
//       record_sz - pointer to the size of this block's record
//       record - points to the first element of the record
// Retn: None
// -----------------------------------------------------------------------------
{
  memcpy(block->prevhash, &blockframe[PREVHASH_POS], HASH_SZ);
  memcpy(block->hash, &blockframe[CURRHASH_POS], HASH_SZ);
  block->index = *(uint64_t *)&blockframe[INDEX_POS];
  block->timestamp = *(uint64_t *)&blockframe[TS_POS];
  block->record_sz = *(uint_fast64_t *)&blockframe[RECORD_SZ_POS];
  memcpy(block->record, &blockframe[RECORD_POS], block->record_sz);
}

// frames a block (stores all members in a buffer with no padding)
void block_frame(Block *this, uint8_t *buf)
// -----------------------------------------------------------------------------
// Func: Store the contents of the block struct in an array to remove 0 buffer
// Args: block - a pointer to this block
//       buf - the buffer that the contents of the block will be copied to
// Retn: None
// -----------------------------------------------------------------------------
{
  memcpy(&buf[PREVHASH_POS], this->prevhash, HASH_SZ);
  memcpy(&buf[CURRHASH_POS], this->hash, HASH_SZ);
  memcpy(&buf[INDEX_POS], &this->index, WORD_SZ);
  memcpy(&buf[TS_POS], &this->timestamp, WORD_SZ);
  memcpy(&buf[RECORD_SZ_POS], &this->record_sz, WORD_SZ);
  memcpy(&buf[RECORD_POS], this->record, this->record_sz);
}

void block_hash(Block *this, uint8_t *hash)
// -----------------------------------------------------------------------------
// Func: Hash the the block after removing struct 0 padding
// Args: this - a pointer to the block
//       hash - a pointer to the hash of the block
// Retn: None
// -----------------------------------------------------------------------------
{
  uint8_t buf[BLOCK_HEADER_SZ + this->record_sz]; // record size is variable
  block_frame(this, buf);
  util_buf_hash(buf, BLOCK_HEADER_SZ + this->record_sz, hash);
}
