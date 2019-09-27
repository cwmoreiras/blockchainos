#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>


// node class
typedef struct Node Node;
struct Node {
  void *contents;
  struct Node *prev; // optional - use for linked list
  struct Node *next; // optional - use for linked list
};

void node_init(Node *node, const void *contents, const uint64_t clen,
               const Node *prev, const Node *next);
void node_destroy(Node *node);

// buffer utilities

void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len);
int util_buf_write_raw(const uint8_t *, int, const char *pathname);

#endif
