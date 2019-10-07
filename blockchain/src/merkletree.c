/*
merkletree.c: method definitions for mertkletree structure
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


void remove_warning(); // just some code in here to suppress a warning

/*

#include "merkletree.h"
#include "node.h"


int merkletree_validate_node(MerkleTree *mtree, Node *node);
/nt merkletree_validate_tree(MerkleTree *mtree, Node *node);
int merkletree_insert_node(MerkleTree *mtree, Node *node);
int merkletree_insert_tree(MerkleTree *mtree, Node *node);

void merkletree_init(MerkleTree *mtree, Node *root) {
  mtree->root = root;
  mtree->validate_node = &merkletree_validate_node;
  mtree->validate_tree = &merkletree_validate_tree;
  mtree->insert_node = &merkletree_insert_node;
  mtree->insert_tree = &merkletree_insert_tree;
}

void merkletree_destroy(MerkleTree *mtree) {
  mtree->validate_node = NULL;
  mtree->validate_tree = NULL;
  mtree->insert_node = NULL;
  mtree->insert_tree = NULL;
}

int merkletree_validate_node(MerkleTree *mtree, Node *node) {
  return 0;
}

int merkletree_validate_tree(MerkleTree *mtree, Node *node) {
  return 0;
}

int merkletree_insert_node(MerkleTree *mtree, Node *node) {
  return 0;
}

int merkletree_insert_tree(MerkleTree *mtree, Node *node) {
  return 0;
}
*/
