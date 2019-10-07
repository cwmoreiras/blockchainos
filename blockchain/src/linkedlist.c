/*
linkedlist.c: method definitions for linkedlist structure
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

#include "linkedlist.h"
#include "node.h"

#include <stdlib.h>

// private functions, access through LinkedList object
void linkedlist_insert_front(LinkedList *this, void *data, uint64_t sz);
void linkedlist_delete_front(LinkedList *this);
void *linkedlist_peek_front(LinkedList *this);
void linkedlist_destroy(LinkedList *this);

void linkedlist_init(LinkedList *this)
// -----------------------------------------------------------------------------
// Func: Initialize the linked list with a head and tail node, both without
//       data. This structure is initialized with an empty head and empty tail
//       pointing at eachother.
// Args: this - a pointer to this linkedlist object
// Retn: none
// -----------------------------------------------------------------------------
{
  this->sz = 0; // no elements
  this->head = malloc(sizeof(struct Node));
  this->tail = malloc(sizeof(struct Node));
  node_init(this->head, NULL, 0); // no data
  node_init(this->tail, NULL, 0); // no data

  // point the head and tail at eachother
  this->head->prev = this->tail;
  this->head->next = NULL;
  this->tail->next = this->head;
  this->tail->prev = NULL;

  // assign function pointers
  this->insert_front = &linkedlist_insert_front;
  this->delete_front = &linkedlist_delete_front;
  this->peek_front = &linkedlist_peek_front;
}

void linkedlist_insert_front(LinkedList *this, void *data, uint64_t sz)
// -----------------------------------------------------------------------------
// Func: Insert data into a node, and then insert the node into the linkedlist.
// Args: this - a pointer to this linkedlist object
//       data - data segment to insert to the list
//       sz - size of the data object in bytes
// Retn: none
// -----------------------------------------------------------------------------
{
  Node *node = malloc(sizeof(struct Node)); // allocate space for a node
  node_init(node, data, sz); // initialize the node with the caller's data

  // insert the node between the last node and the head
  node->prev = this->head->prev;
  this->head->prev->next = node;
  node->next = this->head;
  this->head->prev = node;

  this->sz++;
}

void *linkedlist_peek_front(LinkedList *this)
// -----------------------------------------------------------------------------
// Func: Look at the data contained in the node that was most recently added to
//       this list
// Args: this - a pointer to this linkedlist object
// Retn: a pointer to the data contained in the node at the front of the list
//       (the most recent node). Does NOT return a copy, just a pointer
// -----------------------------------------------------------------------------
{
  return this->head->prev->data;
}

void linkedlist_delete_front(LinkedList *this)
// -----------------------------------------------------------------------------
// Func: Delete the most recent node
// Args: this - a pointer to this linkedlist object
// Retn: None
// -----------------------------------------------------------------------------
{
  // the second to last node is now the last node
  this->head->prev = this->head->prev->prev;

  node_destroy(this->head->prev->next); // destroy the node
  free(this->head->prev->next); // free the node

  this->head->prev->next = this->head; // attach the head to the new final node

  this->sz--;
}

void linkedlist_destroy(LinkedList *this)
// -----------------------------------------------------------------------------
// Func: Destroy the list, free all allocated memory associated with nodes,
//       the data in the nodes, and then the head and tail. Runs in O(n)
// Args: this - a pointer to this linkedlist object
// Retn: None
// -----------------------------------------------------------------------------
{
  // must destroy all user data, nodes, and then head and tail

  // TODO buffer overflow
  while (this->sz > 0) { // O(n)
    this->delete_front(this);
  }

  node_destroy(this->head);
  node_destroy(this->tail);

  free(this->head);
  free(this->tail);

  this->head = NULL;
  this->tail = NULL;

  this->insert_front = NULL;
  this->delete_front = NULL;
  this->peek_front = NULL;
}
