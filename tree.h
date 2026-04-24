#ifndef _TREE_H_
#define _TREE_H_
#include <stdbool.h>

// Data structure for pet information
typedef struct item
{
    char petname[20];
    char petkind[20];
} Item;

#define MAXITEMS 10

// Binary tree node structure
typedef struct trnode
{
    Item item;
    struct trnode * left;  // Pointer to left child
    struct trnode * right; // Pointer to right child
} Trnode;

// Tree management structure
typedef struct tree
{
    Trnode * root; // Pointer to the root node
    int size;      // Current number of items in the tree
} Tree;

// Function prototypes

// Set tree to empty state
void InitializeTree(Tree * ptree);

// Return true if tree is empty
bool TreeIsEmpty(const Tree * ptree);

// Return true if tree is full
bool TreeIsFull(const Tree * ptree);

// Get number of items in the tree
int TreeItemCount(const Tree * ptree);

// Add an item to the tree
bool AddItem(const Item * pi, Tree * ptree);

// Find an item in the tree
bool InTree(const Item * pi, const Tree * ptree);

// Delete an item from the tree
bool DeleteItem(const Item * pi, Tree * ptree);

// Apply a function to each item in the tree
void Traverse(const Tree * ptree, void (*pfun)(Item item));

// Free all allocated memory and clear the tree
void DeleteAll(Tree * ptree);

#endif
