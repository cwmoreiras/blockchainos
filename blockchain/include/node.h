/*
node.h: node structure definition
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


#ifndef NODE_H
#define NODE_H

#include <stdint.h>

// forward declaration
typedef struct Node Node;

// node class
struct Node 
// -----------------------------------------------------------------------------
// Description
//  Generic node class with two pointers that can be used to construct
//  linked lists or trees.
// -----------------------------------------------------------------------------
{
  void *data;
  struct Node *prev; // optional - use for linked list
  struct Node *next; // optional - use for linked list
};

// public methods
void node_init(Node *this, const void *data, const uint64_t sz); // constructor
void node_destroy(Node *this); // destructor

#endif
