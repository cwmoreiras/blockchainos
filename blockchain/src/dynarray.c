#include "dynarray.h"
#include <stdint.h>
#include <stdlib.h>

int dynarray_insert(DynArray *this, uint8_t element, uint64_t index);
uint8_t dynarray_remove(DynArray *this, uint64_t index, int *valid);
int dynarray_set(DynArray *this, uint8_t element, uint64_t index);
uint8_t dynarray_get(DynArray *this, uint64_t index, int *valid);
int dynarray_grow(DynArray *this);

void dynarray_init(DynArray *this, uint64_t cap) {
  this->buf = malloc(cap*sizeof(uint8_t));
  this->sz = 0;
  this->cap = cap;
  this->insert = &dynarray_insert;
  this->remove = &dynarray_remove;
  this->set = &dynarray_set;
  this->get = &dynarray_get;
}

void dynarray_destroy(DynArray *this) {
  free(this->buf);
  this->sz = 0;
  this->cap = 0;
  this->insert = NULL;
  this->remove = NULL;
  this->set = NULL;
  this->get = NULL;
}

int dynarray_grow(DynArray *this) {
  this->cap *= 2;
  if ((this->buf = realloc(this->buf, this->cap*sizeof(void*))) == NULL) {
    return -1; // TODO error stuff
  }

  return 0;
}

int dynarray_insert(DynArray *this, uint8_t element, uint64_t index) {
  uint64_t i;

  if (index > this->sz) {
    return -1; // TODO index out of range
  }

  // if size == cap and grow fails, return -1
  if ((this->sz == this->cap) && dynarray_grow(this)) {
    return -1;
  }

  // TODO shift all elements right
  for (i = this->sz; i > index; i--) {
    this->buf[i] = this->buf[i-1];
  }

  this->buf[index] = element;
  this->sz++;

  return 0;
}

// returns a pointer to the memory that is begin removed from the list
// valid contains the error code
uint8_t dynarray_remove(DynArray *this, uint64_t index, int *valid) {
  uint64_t i;
  uint8_t element;

  if (index >= this->sz)
    *valid = 0;
  *valid = 1;
  element = this->buf[index];

  for (i = index; i < this->sz-1; i++) {
    this->buf[i] = this->buf[i+1];
  }
  this->sz--;

  return element;
}

int dynarray_set(DynArray *this, uint8_t element, uint64_t index) {
  if (index > this->sz) {
    return -1; // TODO index out of range
  }

  this->buf[index] = element;

  return 0;
}

uint8_t dynarray_get(DynArray *this, uint64_t index, int *valid) {
  if (index >= this->sz)
    *valid = 0;
  *valid = 1;

  return this->buf[index];
}
