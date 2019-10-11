/*
blockchain.h: structure definition for block & blockchain
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

#ifndef BLOCK_H
#define BLOCK_H

#include "node.h"
#include "linkedlist.h"

#define BLOCK_HEADER_SZ 88

#define HASH_SZ         32 // SHA256 sum has 32 byte digest
#define WORD_SZ         8  // 8 byte words for high integer counters, etc
                           // probably unecessary?

#define PREVHASH_POS    0
#define CURRHASH_POS    32
#define INDEX_POS       64
#define TS_POS          72
#define RECORD_SZ_POS   80
#define RECORD_POS      88

typedef struct Block Block;
typedef struct Blockchain Blockchain;

struct Block 
// -----------------------------------------------------------------------------
// Description
//  Definition of Block, an implementation of a blockchain block. This class can
//  be used to store the data necessary for creation of a block, but an instance 
//  of this struct is not designed to be stored in the chain directly. Objects
//  of this type should be used to construct BlockFrames, which have no zero
//  padding, and can be stored in the chain.
// -----------------------------------------------------------------------------
{
  uint8_t prevhash[HASH_SZ];
  uint8_t hash[HASH_SZ];
  uint64_t index;
  uint64_t timestamp;
  uint64_t record_sz;
  uint8_t *record;
};

// TODO encapsulate these functions?
void blockframe_decode(uint8_t *this,
                       uint8_t *prevhash, uint8_t *hash,
                       uint64_t *index, uint64_t *timestamp,
                       uint64_t *record_sz, uint8_t *record);
void blockframe_print(uint8_t *this) ;

struct Blockchain
//------------------------------------------------------------------------------
// Description
//  Definition of the Blockchain.  Blockchain is essentially a wrapper for
//  LinkedList, acting as a subclass with altered method implementation.
//  TODO: Better doc
//------------------------------------------------------------------------------
{
  LinkedList *ll;

  // peek_front maps directly to LinkedList->peek_front
  void *(*peek_front)(Blockchain *this);

  // Blockchain->insert_front has different implementation 
  // than LinkedList->insert_front. We can call this append,
  // but this seems more obj oriented if that's desireable
  void (*insert_front)(Blockchain *this,
                 uint8_t *record,
                 uint64_t record_sz);

  int (*verify_block) (Block *new_block, Block *old_block);

  // Can't delete blocks... returns error.  I think we can get rid of this... 
  // unless we want to stress that this is a subclass
  // void (*delete_front)(Blockchain *this);
};

// public methods
void blockchain_init(Blockchain *this); // blockchain contructor
void blockchain_destroy(Blockchain *this); // blockchain destructor

#endif
