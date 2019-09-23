#ifndef BLOCK_H
#define BLOCK_H

typedef struct block {
  int index;
  int prev_hash;
  int timestamp;
  int data;
  int hash;
} block;

#endif
