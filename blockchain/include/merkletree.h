/*
merkletree.h: merkletree structure definition
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

/*

#include "node.h"


typedef struct MerkleTree MerkleTree;

struct MerkleTree {
  Node *root;
  Node **leaves;
  int (*validate_node)(MerkleTree *mtree, Node *node);
  int (*validate_tree)(MerkleTree *mtree, Node *node);
  int (*insert_node)(MerkleTree *mtree, Node *node);
  int (*insert_tree)(MerkleTree *mtree, Node *node);

};
void merkletree_init(MerkleTree *mtree, Node *root);
void merkletree_destroy(MerkleTree *mtree);
*/

#endif
