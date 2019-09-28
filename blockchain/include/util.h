#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

typedef struct DynamicArray DynamicArray;
typedef struct LinkedList LinkedList;
typedef struct Node Node;


// linked list
struct LinkedList {
  Node *head;
  Node *tail;
  uint64_t sz;
  void (*insert_front)(LinkedList *ll, void *contents, uint64_t clen);
  void (*delete_front)(LinkedList *ll);
  void *(*peek_front)(LinkedList *ll);
};
void linkedlist_init(LinkedList *ll);
void linkedlist_destroy(LinkedList *ll);

// node class
struct Node {
  void *contents;
  struct Node *prev; // optional - use for linked list
  struct Node *next; // optional - use for linked list
};

void node_init(Node *node, const void *contents, const uint64_t clen);
void node_destroy(Node *node);

// buffer utilities
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len);
int util_buf_write_raw(const uint8_t *, int, const char *pathname);

#endif
