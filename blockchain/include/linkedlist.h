/*
linkedlist.h: linklist structure definition
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


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"

// linked list
typedef struct LinkedList LinkedList;

struct LinkedList
// -----------------------------------------------------------------------------
// Description
//  Implements a doubly linked list data structure, with limited functionality.
//  This structure uses a head and tail node. By convention, the head is the 
//  "front" of the list, and the tail is the "back" of the list. The only way
//  to interact with this list is from the "front", which will correspond to the
//  node that has most recently been added. Getting an arbitrary element happens
//  in O(n) time.
//
//  For now, this structure contains some methods that are blockchain-only. In
//  the future, these methods will encapsulated such that no linkedlist which 
//  is not also a blockchain will be able to call them. An alternative might be
//  to reassign the function pointers of the blockchain in a blockchain
//  constructor.
// ----------------------------------------------------------------------------- 
{
  Node *head; // will contain null data
  Node *tail; // will contain null data
  uint64_t sz; // tracks the number of elements in the list

  void (*insert_front)(LinkedList *this, void *data, uint64_t sz);
  void (*delete_front)(LinkedList *this);
  void *(*get)(LinkedList *this, uint64_t index);
  void *(*peek_front)(LinkedList *this);
};

// public methods
void linkedlist_init(LinkedList *this); // linkedlist constructor
void linkedlist_destroy(LinkedList *this); // linkedlist destructor

#endif
