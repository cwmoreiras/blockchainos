#include "util.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// private functions, access through LinkedList object
void linkedlist_insert_front(LinkedList *ll, void *contents, uint64_t clen);
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

void linkedlist_insert_front(LinkedList *ll, void *contents, uint64_t clen) {
  Node *node = malloc(sizeof(struct Node));
  node_init(node, contents, clen);

  node->prev = ll->head->prev;
  ll->head->prev->next = node;

  node->next = ll->head;
  ll->head->prev = node;

  ll->sz++;
}

void *linkedlist_peek_front(LinkedList *ll) {
  return ll->head->prev->contents;
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

}

// sets the contents of the node
void node_init(Node *node, const void *contents, const uint64_t clen) {
  if ((node->contents = malloc((size_t)clen)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(node->contents, (void *)contents, clen);

  // point the prev, next pointers at the other nodes
  node->prev= NULL;
  node->next= NULL;
}

void node_destroy(Node *node) {
  free(node->contents);
  node->contents = NULL;
  node->prev = NULL;
  node->next = NULL;
}

int util_buf_write_raw(const uint8_t *buf, int n, const char *pathname) {
  // see man page for open to read about O_CLOEXEC in multithreaded
  // programs
  int fd = open(pathname, O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC);

  if (fd < 0) {
    return fd;
  }

  if (write(fd, buf, n) != n) {
    return -1; // TODO how to do errors
  }

  close(fd);

  return 0;
}

// time  - O(n) array size is tight bound
// space - O(n), exactly 2n
// use temporary array so that the caller can use the same buffer
// for dest and src
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len)
{
  int i;
  uint8_t *temp;

  temp = malloc(len);

  for (i = 0; i < len; i++)
    temp[len-1-i] = src[i]; // reverse the array
  for (i = 0; i < len; i++)
    dest[i] = temp[i]; // copy the reversed array into dest

  free(temp);
}
