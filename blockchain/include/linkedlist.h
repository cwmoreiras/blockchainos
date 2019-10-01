#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

// linked list
typedef struct LinkedList LinkedList;

struct LinkedList {
  Node *head;
  Node *tail;
  uint64_t sz;
  void (*insert_front)(LinkedList *ll, void *data, uint64_t sz);
  void (*delete_front)(LinkedList *ll);
  void *(*peek_front)(LinkedList *ll);
};
void linkedlist_init(LinkedList *ll);
void linkedlist_destroy(LinkedList *ll);

#endif
