#include "util.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// sets the contents of the node
void node_init(Node *node, const void *contents, const uint64_t clen,
               const Node *prev, const Node *next) {
  if ((node->contents = malloc((size_t)clen)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(node->contents, (void *)contents, clen);

  // point the prev, next pointers at the other nodes
  node->prev=(Node *)prev;
  node->next=(Node *)next;
}


void node_destroy(Node *node) {
  free(node->contents);
  node->contents = NULL;
  node->prev = NULL;
  node->next = NULL;
}

int util_buf_write_raw(const uint8_t *buf, int n, const char *pathname) {
  // see man page for open to read about O_CLOEXEC in multithreaded
  // programs
  int fd = open(pathname, O_CREAT|O_WRONLY|O_TRUNC|O_CLOEXEC);

  if (fd < 0) {
    return fd;
  }

  if (write(fd, buf, n) != n) {
    return -1; // TODO how to do errors
  }

  close(fd);

  return 0;
}

// time  - O(n) array size is tight bound
// space - O(n), exactly 2n
// use temporary array so that the caller can use the same buffer
// for dest and src
void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len)
{
  int i;
  uint8_t *temp;

  temp = malloc(len);

  for (i = 0; i < len; i++)
    temp[len-1-i] = src[i]; // reverse the array
  for (i = 0; i < len; i++)
    dest[i] = temp[i]; // copy the reversed array into dest

  free(temp);
}
