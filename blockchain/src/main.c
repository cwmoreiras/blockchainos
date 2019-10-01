#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "blockchain.h"
#include "util.h"
#include "linkedlist.h"
#include "dynarray.h"


#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

void ll_test() {
  LinkedList chain;

  char *s;
  char *msg0 = "my first block will be this message";
  size_t msg0_sz = strlen(msg0)+1;

  char *msg1 = "my second block will be this message";
  size_t msg1_sz = strlen(msg1)+1;

  linkedlist_init(&chain);

  chain.insert_front(&chain, msg0, msg0_sz);
  s = (char *)chain.peek_front(&chain);
  printf("%s\n", s);

  chain.insert_front(&chain, msg1, msg1_sz);
  s = (char *)chain.peek_front(&chain);
  printf("%s\n", s);

  chain.delete_front(&chain);
  s = (char *)chain.peek_front(&chain);
  printf("%s\n", s);

  linkedlist_destroy(&chain);
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

// should i encapsulate node in linkedlist?

int main() {
  Blockchain bc;
  char *str = "this is the second block";
  uint64_t str_sz = strlen(str)+1;

  blockchain_init(&bc);
  printf("%s\n", (char *)bc.ll->peek_front(bc.ll));

  blockchain_destroy(&bc);


}
