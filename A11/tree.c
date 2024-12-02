#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node* find(const char* name, struct tree_node* root)
{
  if (root == NULL) {
    // Returns NULL if tree is empty or if key is not found
    return NULL;
  }
  
  int cmp = strcmp(name, root->data.name);
  if (cmp < 0) {
    // Search in the left sub-tree
    return find(name, root->left);
  } else if (cmp > 0) {
    // Search in the right sub-tree
    return find(name, root->right);
  } else {
    // Key is found, return the node
    return root;
  }
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
  if (root == NULL) {
    // Create new node until we reach a leaf node
    struct tree_node* new_node = (struct tree_node*) malloc(sizeof(struct tree_node));
    if (!new_node) {
      perror("Failed to allocate memory for new tree node");
      return NULL;
    }
    // Copy the name
    strncpy(new_node->data.name, name, sizeof(new_node->data.name) - 1);
    new_node->data.name[sizeof(new_node->data.name) -1] = '\0';
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
  }

  // Compare the given name with the node's name
  int cmp = strcmp(name, root->data.name);
  if (cmp < 0) {
    // Insert into left sub-tree
    root->left = insert(name, root->left);
  } else if (cmp > 0) {
    // Insert into right sub-tree
    root->right = insert(name, root->right);
  }

  // If the name is already in the tree, return the current root
  return root;
}

void clear(struct tree_node* root)
{
  if (root != NULL){
    // Clear both subtrees, then free node
    clear(root->left);
    clear(root->right);
    free(root);
  }
}

void print(struct tree_node* root)
{
  if (root != NULL) {
    // Print the node's name
    printf("%s\n", root->data.name);
    print(root->left); // Print the left sub-tree
    print(root->right); // Print the right sub-tree
  }
}

void printSorted(struct tree_node* root)
{
  // Tree printed in in-order traversal
  if (root != NULL) {
    printSorted(root->left);
    printf("%s\n", root->data.name);
    printSorted(root->right);
  }
}

