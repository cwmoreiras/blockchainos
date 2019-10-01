#include "linkedlist.h"
#include "node.h"

#include <stdlib.h>

// private functions, access through LinkedList object
void linkedlist_insert_front(LinkedList *ll, void *data, uint64_t sz);
void linkedlist_delete_front(LinkedList *ll);
void *linkedlist_peek_front(LinkedList *ll);
void linkedlist_destroy(LinkedList *ll);

void linkedlist_init(LinkedList *ll) {
  ll->sz = 0;

  ll->head = malloc(sizeof(struct Node));
  ll->tail = malloc(sizeof(struct Node));
  node_init(ll->head, NULL, 0);
  node_init(ll->tail, NULL, 0);

  // point the head and tail at eachother
  ll->head->prev = ll->tail;
  ll->head->next = NULL;
  ll->tail->next = ll->head;
  ll->tail->prev = NULL;

  // assign function pointers
  ll->insert_front = &linkedlist_insert_front;
  ll->delete_front = &linkedlist_delete_front;
  ll->peek_front = &linkedlist_peek_front;
}

void linkedlist_insert_front(LinkedList *ll, void *data, uint64_t sz) {
  Node *node = malloc(sizeof(struct Node));
  node_init(node, data, sz);

  node->prev = ll->head->prev;
  ll->head->prev->next = node;

  node->next = ll->head;
  ll->head->prev = node;

  ll->sz++;
}

void *linkedlist_peek_front(LinkedList *ll) {
  return ll->head->prev->data;
}

void linkedlist_delete_front(LinkedList *ll) {
  // turn the front element into the head
  ll->head->prev = ll->head->prev->prev;

  node_destroy(ll->head->prev->next);
  free(ll->head->prev->next);
  ll->head->prev->next = ll->head;

  ll->sz--;
}

void linkedlist_destroy(LinkedList *ll) {
  // must destroy all user data, nodes, and then head and tail
  while (ll->sz > 0) {
    ll->delete_front(ll);
  }
  node_destroy(ll->head);
  node_destroy(ll->tail);

  free(ll->head);
  free(ll->tail);

  ll->head = NULL;
  ll->tail = NULL;

  ll->insert_front = NULL;
  ll->delete_front = NULL;
  ll->peek_front = NULL;

}
