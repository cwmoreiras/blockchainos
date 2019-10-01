#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdint.h>

typedef struct DynArray DynArray;

struct DynArray {
  uint8_t *buf;
  uint64_t sz;
  uint64_t cap;
  int (*insert)(DynArray *this, uint8_t element, uint64_t index);
  uint8_t (*remove)(DynArray *this, uint64_t index, int *valid);
  int (*set)(DynArray *this, uint8_t element, uint64_t index);
  uint8_t (*get)(DynArray *this, uint64_t index, int *valid);
};
void dynarray_init(DynArray *this, uint64_t cap);
void dynarray_destroy(DynArray *this);

#endif
