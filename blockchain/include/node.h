#ifndef NODE_H
#define NODE_H

#include <stdint.h>

typedef struct Node Node;

// node class
struct Node {
  void *data;
  struct Node *prev; // optional - use for linked list
  struct Node *next; // optional - use for linked list
};

void node_init(Node *node, const void *data, const uint64_t sz);
void node_destroy(Node *node);

#endif
