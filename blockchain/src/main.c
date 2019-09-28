#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"
#include "util.h"

int main() {
  LinkedList *ll;
  char ch = 'a';
  char rv;

  ll = malloc(sizeof(struct LinkedList));
  linkedlist_init(ll);

  ll->insert_front(ll, &ch, 1);
  rv = *(char*)(ll->peek_front(ll));
  printf("rv: %c\n", rv);
  ll->delete_front(ll);
  linkedlist_destroy(ll);

  free(ll);
}
