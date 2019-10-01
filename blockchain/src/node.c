#include "node.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void node_init(Node *this, const void *data, const uint64_t sz) {
  if ((this->data = malloc((size_t)sz)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(this->data, (void *)data, sz);

  // point the prev, next pointers at the other nodes
  this->prev= NULL;
  this->next= NULL;
}

void node_destroy(Node *this) {
  free(this->data);
  this->data = NULL;
  this->prev = NULL;
  this->next = NULL;
}
