#ifndef optimalBST_H_
#define optimalBST_H_

#include <stdio.h>

typedef struct BST {
    int BSTroot;
    int **BSTtree;
    int **BSTcost;
    size_t n;
} BST;

#endif