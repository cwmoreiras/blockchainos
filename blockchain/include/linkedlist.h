#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

// linked list
typedef struct LinkedList LinkedList;

struct LinkedList {
  Node *head;
  Node *tail;
  uint64_t sz;
  void (*insert_front)(LinkedList *this, void *data, uint64_t sz);
  void (*delete_front)(LinkedList *this);
  void *(*peek_front)(LinkedList *this);

  // these members are blockchain only
  void (*append)(LinkedList *this,
                 uint8_t *record,
                 uint64_t record_sz);
};
void linkedlist_init(LinkedList *this);
void linkedlist_destroy(LinkedList *this);

#endif
