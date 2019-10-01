#include "node.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void node_init(Node *node, const void *data, const uint64_t sz) {
  if ((node->data = malloc((size_t)sz)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(node->data, (void *)data, sz);

  // point the prev, next pointers at the other nodes
  node->prev= NULL;
  node->next= NULL;
}

void node_destroy(Node *node) {
  free(node->data);
  node->data = NULL;
  node->prev = NULL;
  node->next = NULL;
}
