#include "util.h"
#include <stdlib.h>

void util_reverse_buf(unsigned char *dest, const unsigned char *src,
                      const int len)
{
  int i;
  unsigned char *temp;

  temp = malloc(len);

  for (i = 0; i < len; i++)
    temp[len-1-i] = src[i];
  for (i = 0; i < len; i++)
    dest[i] = temp[i];

  free(temp);
}
