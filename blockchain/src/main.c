#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"
#include "util.h"


#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

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
  int valid;
  uint8_t rv;
  uint8_t val1, val2, val3;
  uint64_t i;

  da = malloc(sizeof(struct DynArray));
  dynarray_init(da, 2);

  val1 = 10;
  da->insert(da, val1, 0);

  val2 = 20;
  da->insert(da, val2, 1);

  val3 = 30;
  da->insert(da, val3, 2);

  da->remove(da, 1, &valid);

  for (i = 0; i < da->sz; i++) {
    rv = da->get(da, i, &valid);
    printf("i:%ld , val:%d\n",i, rv);
  }



  dynarray_destroy(da);
  free(da);
}

void block_test() {

}

int main() {
  da_test();
  ll_test();
  printf("%d\n", SHA256_DIGEST_LENGTH);
}
