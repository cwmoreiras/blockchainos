#include "node.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void node_init(Node *this, const void *data, const uint64_t sz)
// -----------------------------------------------------------------------------
// Func: Initialize the node's data segment on the heap and copy the caller's
//       data into it, so that the caller can free their memory
// Args: this - a pointer to this node object
//       data - pointer to the caller's data
//       sz - size of the data block in bytes
// Retn: none
// -----------------------------------------------------------------------------
{
  if ((this->data = malloc((size_t)sz)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(this->data, (void *)data, sz);

  this->prev= NULL;
  this->next= NULL;
}

void node_destroy(Node *this)
// -----------------------------------------------------------------------------
// Func: Free the data this node is storing.
// Args: this - a pointer to this node object
// Retn: none
// -----------------------------------------------------------------------------
{
  free(this->data);
  this->data = NULL;
  this->prev = NULL;
  this->next = NULL;
}
