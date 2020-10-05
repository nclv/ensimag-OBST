#include "optimalBST.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * Initialise le BST
 */
struct BST *initialisation_BST(size_t n) {
    struct BST *optimal = malloc(sizeof(BST));
    optimal->n = n;
    optimal->BSTtree = malloc(n * sizeof(int *));
    optimal->BSTcost = malloc(n * sizeof(int *));
    for (size_t i = 0; i < n; i++) {
        optimal->BSTtree[i] = malloc(2 * sizeof(int));
        optimal->BSTcost[i] = malloc(n * sizeof(int));
    }
    return optimal;
}

/**
 * Optimal Binary Search Tree
 */
struct BST *optimalSearchTree(double *sommes_p, size_t n) {
    struct BST *optimal = initialisation_BST(n);
    afficher_tableau(sommes_p, n);

    return optimal;
}