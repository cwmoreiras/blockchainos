
void remove_warning();

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
