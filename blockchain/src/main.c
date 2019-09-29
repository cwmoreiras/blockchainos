#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"
#include "util.h"

void ll_test() {
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

void da_test() {
  DynArray *da;
  int val = 5;
  int valid;
  int *rv;

  da = malloc(sizeof(struct DynArray));
  dynarray_init(da, 100);

  da->insert(da, &val, 0);
  rv = (int *)da->get(da, 0, &valid);

  printf("%d\n", *rv);


  dynarray_destroy(da);
  free(da);
}

int main() {

  da_test();
}
