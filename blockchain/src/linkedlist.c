#include "linkedlist.h"
#include "node.h"

#include <stdlib.h>

// private functions, access through LinkedList object
void linkedlist_insert_front(LinkedList *this, void *data, uint64_t sz);
void linkedlist_delete_front(LinkedList *this);
void *linkedlist_peek_front(LinkedList *this);
void linkedlist_destroy(LinkedList *this);


void linkedlist_init(LinkedList *this) {
  this->sz = 0;
  this->head = malloc(sizeof(struct Node));
  this->tail = malloc(sizeof(struct Node));
  node_init(this->head, NULL, 0);
  node_init(this->tail, NULL, 0);

  // point the head and tail at eachother
  this->head->prev = this->tail;
  this->head->next = NULL;
  this->tail->next = this->head;
  this->tail->prev = NULL;

  // assign function pointers
  this->insert_front = &linkedlist_insert_front;
  this->delete_front = &linkedlist_delete_front;
  this->peek_front = &linkedlist_peek_front;
}

void linkedlist_insert_front(LinkedList *this, void *data, uint64_t sz) {
  Node *node = malloc(sizeof(struct Node));
  node_init(node, data, sz);

  node->prev = this->head->prev;
  this->head->prev->next = node;

  node->next = this->head;
  this->head->prev = node;

  this->sz++;
}

void *linkedlist_peek_front(LinkedList *this) {
  return this->head->prev->data;
}

void linkedlist_delete_front(LinkedList *this) {
  // turn the front element into the head
  this->head->prev = this->head->prev->prev;

  node_destroy(this->head->prev->next);
  free(this->head->prev->next);
  this->head->prev->next = this->head;

  this->sz--;
}

void linkedlist_destroy(LinkedList *this) {
  // must destroy all user data, nodes, and then head and tail
  while (this->sz > 0) {
    this->delete_front(this);
  }
  node_destroy(this->head);
  node_destroy(this->tail);

  free(this->head);
  free(this->tail);

  this->head = NULL;
  this->tail = NULL;

  this->insert_front = NULL;
  this->delete_front = NULL;
  this->peek_front = NULL;
}
