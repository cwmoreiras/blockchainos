#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

// buffer utilities

void util_buf_reverse(uint8_t *dest, const uint8_t *src,
                      const int len);
int util_buf_write_raw(const uint8_t *, int, const char *pathname);


#endif
