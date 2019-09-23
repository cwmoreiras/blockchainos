#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "main.h"
#include "block.h"
#include "blockchain.h"
#include "crypto.h"

int main() {
  block *b;

  b = malloc(sizeof(block));
  generator(b);

  free(b);
}
