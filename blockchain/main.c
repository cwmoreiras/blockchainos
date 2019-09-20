#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "blockchain.h"
#include "main.h"

int main() {
  block *b;

  b = malloc(sizeof(block));
  generator(b);

  free(b);
}
