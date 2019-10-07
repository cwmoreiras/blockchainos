/*
main.c: main program. launches whatever necessary data structures
        and calls test functions
Copyright (C) 2019 
maintainer: Carlos WM
email: cwmoreiras@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

  util_print_license();

  uint8_t *blockframe;
  uint8_t msg[1000];
  uint64_t msglen;

  uint8_t *record = (uint8_t *) "this is the second block";
  uint64_t record_sz = strlen((char *)record)+1;

  Blockchain bc;

  blockchain_init(&bc);
  blockframe = (uint8_t *) bc.peek_front(&bc); // returns a framed block
  blockframe_decode(blockframe, &msglen, msg);
  printf("%s\n", msg);

  bc.append(&bc, record, record_sz);
  blockframe = (uint8_t *) bc.peek_front(&bc); // returns a framed block
  blockframe_decode(blockframe, &msglen, msg);
  printf("%s\n", msg);


  blockchain_destroy(&bc);
}
