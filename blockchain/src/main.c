#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"
#include "util.h"

int main() {
  uint8_t ch = 'A';
  Node *node;
  LinkedList *ll;

  node = malloc(sizeof(struct Node));
  node_init(node, &ch, 1);

  ll = malloc(sizeof(struct LinkedList));
  linkedlist_init(ll);

  ll->insert_front(ll, node);
  char *res = (char*) ll->peek_front(ll)->contents;

  printf("%c\n", *res);
  printf("%ld\n", ll->sz);

  free(node);
  free(ll);
}
