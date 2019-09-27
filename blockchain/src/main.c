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

  node = malloc(sizeof(struct Node));
  node_init(node, &ch, 1, (Node *)NULL, (Node *)NULL);
  node_destroy(node);
  free(node);
}
