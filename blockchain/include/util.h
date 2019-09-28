#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef struct DynArray DynArray;
typedef struct LinkedList LinkedList;
typedef struct Node Node;

struct DynArray {
  void *buf;
  uint64_t sz;
  uint64_t cap;
  int (*insert)(DynArray *da, void *element, uint64_t index);
  int (*remove)(DynArray *da, uint64_t index);
  int (*set)(DynArray *da, void *element, uint64_t index);
  int (*get)(DynArray *da, uint64_t index);
};
void dynarray_init(DynArray *da, uint64_t cap);
void dynarray_destroy(DynArray *da);

// linked list
struct LinkedList {
  Node *head;
  Node *tail;
  uint64_t sz;
  void (*insert_front)(LinkedList *ll, void *data, uint64_t dlen);
  void (*delete_front)(LinkedList *ll);
  void *(*peek_front)(LinkedList *ll);
};
void linkedlist_init(LinkedList *ll);
void linkedlist_destroy(LinkedList *ll);

// node class
struct Node {
  void *data;
  struct Node *prev; // optional - use for linked list
  struct Node *next; // optional - use for linked list
};

void node_init(Node *node, const void *data, const uint64_t dlen);
void node_destroy(Node *node);

// buffer utilities
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len);
int util_buf_write_raw(const uint8_t *, int, const char *pathname);

#endif
