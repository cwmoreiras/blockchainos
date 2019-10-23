/*
dynarray.h: dynarray structure definition
Copyright (C) 2019 
maintainer: Carlos WM
email: cwmoreiras@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdint.h>

// forward declaration
typedef struct DynArray DynArray;

struct DynArray 
// -----------------------------------------------------------------------------
// Description
//  Definition of DynArray, an implementation of a dynamic array data structure,
//  similar to a C++ vector, Java ArrayList, or Python List. This implementation 
//  doesn't have as many methods as those. Methods can be added as the need for
//  increased functionality arises.
// -----------------------------------------------------------------------------
{
  uint8_t *buf; // storage buffer
  uint64_t sz;  // number of elements in the buffer
  uint64_t cap; // capacity of the buffer

  // member functions
  int (*insert)(DynArray *this, uint8_t element, uint64_t index);
  int (*remove)(DynArray *this, uint64_t index, uint8_t *element);
  int (*set)(DynArray *this, uint8_t element, uint64_t index);
  int (*get)(DynArray *this, uint64_t index, uint8_t *element);
};
void dynarray_init(DynArray *this, uint64_t cap);
void dynarray_destroy(DynArray *this);

#endif
