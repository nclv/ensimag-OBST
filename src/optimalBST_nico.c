#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "optimalBST_nico.h"

/** 
 * La valeur à l'emplacement (i, j) dans la matrice triangulaire supérieure se trouve 
 * dans la mémoire à l'emplacement (i * (2 * (n - 1) - i + 1)) / 2 + j.
*/
#define sommes_p(i, j, n) (sommes_p[((i) * (2 * ((n) - 1) - (i) + 1)) / 2 + (j)])


/**
 * Calcul des probabilitées et stockage dans un array 1d
 *
 * @param probabilites tableau des probabilités
 * @param n taille de la matrice carrée générée, n == len(probabilites) + 1
 */
double *calculer_sommes_all(double *probabilites, size_t n) {
    /* On a n * (n + 1) / 2 éléments dans la matrice triangulaire supérieure avec la diagonale centrale nulle */
    double *sommes_p = malloc((n * (n + 1)) / 2 * sizeof(double));
    /* Calcul des sommes des probabilités */
    for (size_t i = 0; i < n; ++i) {
        /* Initialisation de la diagonale à p_0 */
        sommes_p(i, i, n) = 0;
        for (size_t j = i + 1; j < n; ++j) {
            sommes_p(i, j, n) = sommes_p(i, j - 1, n) + probabilites[j - 1];
        }
    }
    return sommes_p;
}

/**
 * Affichage de la matrice triangulaire supérieure de taille n stockée dans array
 *
 * @param array tableau de taille (n * (n + 1)) / 2
 * @param n taille de la matrice carrée
 */
void afficher_tableau_all(double* array, size_t n) {
    size_t line = 1;
    size_t col = 0;
    for (size_t i = 0; i < (n * (n + 1)) / 2; ++i) {
        printf("%lf - ", array[i]);
        if (col == n - line) {
            ++line;
            col = 0;
            printf("\n");
            continue;
        }
        ++col;
	}
	printf("\n");
}