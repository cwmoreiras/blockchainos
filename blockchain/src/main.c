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

// TODO: dynarray users have to be careful with memory allocation
// dynarray doesn't make local copies of elements.
// this makes it dangerous for the caller, because if they assign the same
// address to multiple array elements on accident, multiple dynarray elements
// will be updated when the data in that location is changed.
// This can happen, for example, if the caller sets an element to point at
// an address local to them, then changes the value at the local address, then
// assigns that address to a different element. In that example, both elements
// will be changed, which is probably undesirable.
//
// callers are discouraged from assigning dynarray elements to point at any
// stack allocated memory
//
// callers should also make sure they are assigning each element to a distinct
// address
void da_test() {
  DynArray *da;
  int valid;
  int *rv;
  int val1, val2, val3;
  int i;

  da = malloc(sizeof(struct DynArray));
  dynarray_init(da, 2);

  val1 = 1234;
  da->insert(da, &val1, 0);

  val2 = 1235;
  da->insert(da, &val2, 1);

  val3 = 1236;
  da->insert(da, &val3, 2);

  da->remove(da, 1, &valid);

  for (i = 0; i < da->sz; i++) {
    rv = da->get(da, i, &valid);
    printf("i:%d , val:%d\n",i, *rv);
  }



  dynarray_destroy(da);
  free(da);
}

int main() {

  da_test();
}
