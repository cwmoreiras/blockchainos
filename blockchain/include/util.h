#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void util_reverse_buf(uint8_t *dest, const uint8_t *src,
                      const int len);

#endif
