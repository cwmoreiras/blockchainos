#include "blockchain.h"

void generator(block *b) {
  b->index = 0;
  b->prev_hash = 0;
  b->timestamp = 0;
  b->data = 0;
  b->hash = 0;
}

void append(block *b) {

}
