#ifndef optimalBST_adrien_H_
#define optimalBST_adrien_H_

#include <stdio.h>

#define sommes_p(i, j) (sommes_p[j] - sommes_p[i])

typedef struct BST {
	int BSTroot;
	int **BSTtree;
	int **BSTcost;
	size_t n;
} BST;

extern double *calculer_sommes(double *probabilites, size_t taille);
extern double *lire_fichier(FILE* freqFile, size_t taille);
extern void afficher_tableau(double* tableau, size_t taille);

#endif
