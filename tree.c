#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

// Structure to hold a parent-child node pair for search results
typedef struct pair
{
    Trnode * parent;
    Trnode * child;
} Pair;

// Local function prototypes
static Trnode * MakeNode(const Item * pi);
static bool ToLeft(const Item * i1, const Item * i2);
static bool ToRight(const Item * i1, const Item * i2);
static void AddNode(Trnode * new_node, Trnode * root);
static void InOrder(const Trnode * root, void (*pfun)(Item item));
static Pair SeekItem(const Item * pi, const Tree * ptree);
static void DeleteNode(Trnode **ptr);
static void DeleteAllNodes(Trnode * ptr);

// Initialize tree to an empty state
void InitializeTree(Tree * ptree)
{
    ptree->root = NULL;
    ptree->size = 0;
}

// Return true if the tree is empty
bool TreeIsEmpty(const Tree * ptree)
{
    return (ptree->root == NULL);
}

// Return true if the tree is full (reached MAXITEMS)
bool TreeIsFull(const Tree * ptree)
{
    return (ptree->size == MAXITEMS);
}

// Return the current number of items in the tree
int TreeItemCount(const Tree * ptree)
{
    return ptree->size;
}

// Add an item to the tree; returns true if successful
bool AddItem(const Item * pi, Tree * ptree)
{
    Trnode * new_node;

    if (TreeIsFull(ptree))
    {
        fprintf(stderr, "Tree is full\n");
        return false;
    }

    if (SeekItem(pi, ptree).child != NULL)
    {
        fprintf(stderr, "Attempted to add duplicate item\n");
        return false;
    }

    new_node = MakeNode(pi);
    if (new_node == NULL)
    {
        fprintf(stderr, "Could not create node\n");
        return false;
    }

    ptree->size++;

    if (ptree->root == NULL)
        ptree->root = new_node;
    else
        AddNode(new_node, ptree->root);

    return true;
}

// Search for an item in the tree
bool InTree(const Item * pi, const Tree * ptree)
{
    return (SeekItem(pi, ptree).child != NULL);
}

// Delete an item from the tree; returns true if successful
bool DeleteItem(const Item * pi, Tree * ptree)
{
    Pair look;
    look = SeekItem(pi, ptree);
    if (look.child == NULL)
        return false;

    if (look.parent == NULL) // Delete root node
        DeleteNode(&ptree->root);
    else if (look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);

    ptree->size--;
    return true;
}

// Traverse the tree and apply a function to each item
void Traverse(const Tree * ptree, void (*pfun)(Item item))
{
    if (ptree != NULL)
        InOrder(ptree->root, pfun);
}

// Delete all items and reset the tree
void DeleteAll(Tree * ptree)
{
    if (ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}

// Recursive in-order traversal
static void InOrder(const Trnode * root, void (*pfun)(Item item))
{
    if (root != NULL)
    {
        InOrder(root->left, pfun);
        (*pfun)(root->item);
        InOrder(root->right, pfun);
    }
}

// Recursively delete all nodes and free memory
static void DeleteAllNodes(Trnode * root)
{
    if (root != NULL)
    {
        Trnode * pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}

// Insert a new node into the correct position
static void AddNode(Trnode * new_node, Trnode * root)
{
    if (ToLeft(&new_node->item, &root->item))
    {
        if (root->left == NULL)
            root->left = new_node;
        else
            AddNode(new_node, root->left);
    }
    else if(ToRight(&new_node->item, &root->item))
    {
        if (root->right == NULL)
            root->right = new_node;
        else
            AddNode(new_node, root->right);
    }
    else
    {
        fprintf(stderr, "Location error in AddNode()\n");
        exit(1);
    }
}

// Comparison logic for sorting: returns true if i1 should be on the left of i2
static bool ToLeft(const Item * i1, const Item * i2)
{
    int comp1 = strcmp(i1->petname, i2->petname);
    if (comp1 < 0)
        return true;
    else if (comp1 == 0 && strcmp(i1->petkind, i2->petkind) < 0)
        return true;
    else
        return false;
}

// Comparison logic for sorting: returns true if i1 should be on the right of i2
static bool ToRight(const Item * i1, const Item * i2)
{
    int comp1 = strcmp(i1->petname, i2->petname);
    if (comp1 > 0)
        return true;
    else if (comp1 == 0 && strcmp(i1->petkind, i2->petkind) > 0)
        return true;
    else
        return false;
}

// Allocate memory and initialize a new node
static Trnode * MakeNode(const Item * pi)
{
    Trnode * new_node = (Trnode*)malloc(sizeof(Trnode));
    if (new_node != NULL)
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

// Locate an item and its parent in the tree
static Pair SeekItem(const Item * pi, const Tree * ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;

    while (look.child != NULL)
    {
        if (ToLeft(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(pi, &(look.child->item)))
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
            break; // Found the item
    }

    return look;
}

// Handle node removal and re-attach subtrees
static void DeleteNode(Trnode **ptr)
{
    Trnode * temp;

    if ((*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else // Node has two children
    {
        // Find where to re-attach right subtree
        for (temp = (*ptr)->left; temp->right != NULL; temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}
