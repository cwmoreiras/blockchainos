#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "main.h"
#include "block.h"

int main() {
  block *b;
  /*
  printf("sizeof unsighed char:      %ld\n", sizeof(unsigned char));
  printf("sizeof unsigned int:       %ld\n",  sizeof(unsigned int));
  printf("sizeof unsigned long long: %ld\n", sizeof(unsigned long long));

  printf("sizeof uint8_t:            %ld\n", sizeof(uint8_t));
  printf("sizeof uint32_t:           %ld\n",  sizeof(unsigned int));
  printf("sizeof uint64_t:           %ld\n", sizeof(unsigned long long));
  */

  b = malloc(sizeof(block));
  block_genesis(b);
  block_print(b);

  free(b);
}
