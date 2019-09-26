#include "util.h"
#include <stdlib.h>
#include <stdint.h>

void util_reverse_buf(uint8_t *dest, const uint8_t *src,
                      const int len)
{
  int i;
  uint8_t *temp;

  temp = malloc(len);

  for (i = 0; i < len; i++)
    temp[len-1-i] = src[i];
  for (i = 0; i < len; i++)
    dest[i] = temp[i];

  free(temp);
}
