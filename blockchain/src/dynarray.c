#include "dynarray.h"
#include <stdint.h>
#include <stdlib.h>

int dynarray_insert(DynArray *da, uint8_t element, uint64_t index);
uint8_t dynarray_remove(DynArray *da, uint64_t index, int *valid);
int dynarray_set(DynArray *da, uint8_t element, uint64_t index);
uint8_t dynarray_get(DynArray *da, uint64_t index, int *valid);
int dynarray_grow(DynArray *da);

void dynarray_init(DynArray *da, uint64_t cap) {
  da->buf = malloc(cap*sizeof(uint8_t));
  da->sz = 0;
  da->cap = cap;
  da->insert = &dynarray_insert;
  da->remove = &dynarray_remove;
  da->set = &dynarray_set;
  da->get = &dynarray_get;
}

void dynarray_destroy(DynArray *da) {
  free(da->buf);
  da->sz = 0;
  da->cap = 0;
  da->insert = NULL;
  da->remove = NULL;
  da->set = NULL;
  da->get = NULL;
}

int dynarray_grow(DynArray *da) {
  da->cap *= 2;
  if ((da->buf = realloc(da->buf, da->cap*sizeof(void*))) == NULL) {
    return -1; // TODO error stuff
  }

  return 0;
}

int dynarray_insert(DynArray *da, uint8_t element, uint64_t index) {
  uint64_t i;

  if (index > da->sz) {
    return -1; // TODO index out of range
  }

  // if size == cap and grow fails, return -1
  if ((da->sz == da->cap) && dynarray_grow(da)) {
    return -1;
  }

  // TODO shift all elements right
  for (i = da->sz; i > index; i--) {
    da->buf[i] = da->buf[i-1];
  }

  da->buf[index] = element;
  da->sz++;

  return 0;
}

// returns a pointer to the memory that is begin removed from the list
// valid contains the error code
uint8_t dynarray_remove(DynArray *da, uint64_t index, int *valid) {
  uint64_t i;
  uint8_t element;

  if (index >= da->sz)
    *valid = 0;
  *valid = 1;
  element = da->buf[index];

  for (i = index; i < da->sz-1; i++) {
    da->buf[i] = da->buf[i+1];
  }
  da->sz--;

  return element;
}

int dynarray_set(DynArray *da, uint8_t element, uint64_t index) {
  if (index > da->sz) {
    return -1; // TODO index out of range
  }

  da->buf[index] = element;

  return 0;
}

uint8_t dynarray_get(DynArray *da, uint64_t index, int *valid) {
  if (index >= da->sz)
    *valid = 0;
  *valid = 1;

  return da->buf[index];
}
