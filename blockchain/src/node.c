/*
node.c: method definitions for node structure
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

#include "node.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void node_init(Node *this, const void *data, const uint64_t sz)
// -----------------------------------------------------------------------------
// Func: Initialize the node's data segment on the heap and copy the caller's
//       data into it, so that the caller can free their memory
// Args: this - a pointer to this node object
//       data - pointer to the caller's data
//       sz - size of the data block in bytes
// Retn: none
// -----------------------------------------------------------------------------
{
  if ((this->data = malloc((size_t)sz)) == NULL) {
    exit(1); // TODO critical failure
  }

  // do a copy so that the caller can free their memory
  memcpy(this->data, (void *)data, sz);

  this->prev= NULL;
  this->next= NULL;
}

void node_destroy(Node *this)
// -----------------------------------------------------------------------------
// Func: Free the data this node is storing.
// Args: this - a pointer to this node object
// Retn: none
// -----------------------------------------------------------------------------
{
  free(this->data);
  this->data = NULL;
  this->prev = NULL;
  this->next = NULL;
}
