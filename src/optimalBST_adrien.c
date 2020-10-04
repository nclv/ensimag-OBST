#include <stdio.h>
#include <stdlib.h>

#include "optimalBST_adrien.h"


/**
 * Calcule la somme des probabilités et utilise la macro
 * sommes_p(i, j) pour calculer la somme partielle de i à j
 */
double *calculer_sommes(double *probabilites, size_t taille) {
	double *sommes_p = malloc(taille * sizeof(double));
	double somme = 0;
	for (size_t i = 0; i < taille; ++i) {
		somme += probabilites[i];
		sommes_p[i] = somme;
	}
	return sommes_p;
}

/**
 * Lit le fichier et stocke les probabilités associées
 */
double *lire_fichier(FILE *freqFile, size_t taille) {
	long somme = 0;
	long nombre;
	double *probabilites = malloc(taille * sizeof(double));
	for (size_t i = 0; i < taille; ++i) {
		fscanf(freqFile, "%ld", &nombre);
		probabilites[i] = (double) nombre;
		somme += nombre;
	}
	for (size_t i = 0; i < taille; ++i) {
		probabilites[i] /= (double) somme;
	}
	return probabilites;
}

/**
 * Initialise le BST
 */
struct BST *initialisation_BST(size_t taille) {
	struct BST *optimal = malloc(sizeof(BST));
	optimal->n = taille;
	optimal->BSTtree = malloc(taille * sizeof(int *));
	optimal->BSTcost = malloc(taille * sizeof(int *));
	for (size_t i = 0; i < taille; i++) {
		optimal->BSTtree[i] = malloc(2 * sizeof(int));
		optimal->BSTcost[i] = malloc(taille * sizeof(int));
	}
	return optimal;
}

/**
 * Optimal Binary Search Tree
 */
struct BST *optimalSearchTree(double *sommes_p, size_t taille) {
	struct BST *optimal = initialisation_BST(taille);
    afficher_tableau(sommes_p, taille);
	
	return optimal;
}

/**
 * Affiche un tableau de double
 */
void afficher_tableau(double* tableau, size_t taille) {
	for (size_t i = 0; i < taille; ++i) {
		printf("%lf - ", tableau[i]);
	}
	printf("\n");
}