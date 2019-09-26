#include "util.h"
#include <stdlib.h>
#include <stdint.h>

// time  - O(n) array size is tight bound
// space - O(n), exactly 2n
void util_reverse_buf(uint8_t *dest, const uint8_t *src,
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
