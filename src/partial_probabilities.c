#include "partial_probabilities.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Calcul des probabilitées et stockage dans un array 1d. 
 * Complexité spatiale et temporelle en O((n * (n + 1)) / 2).
 * 
 * Utiliser la macro sommes_p_trig(i, j, n) pour obtenir la somme 
 * partielle de i à j.
 *
 * @param probabilites tableau des probabilités
 * @param n taille de la matrice carrée générée, n == len(probabilites) + 1
 * @return matrice triangulaire supérieure contenant toutes les sommes partielles
 */
double *calculer_sommes_all(double *probabilites, size_t n) {
    /* On a n * (n + 1) / 2 éléments dans la matrice triangulaire supérieure avec la diagonale centrale nulle */
    double *sommes_p = malloc((n * (n + 1)) / 2 * sizeof(double));
    /* Calcul des sommes des probabilités */
    for (size_t i = 0; i < n; ++i) {
        /* Initialisation de la diagonale à p_0 */
        sommes_p_trig(i, i, n) = 0;
        for (size_t j = i + 1; j < n; ++j) {
            sommes_p_trig(i, j, n) = sommes_p_trig(i, j - 1, n) + probabilites[j - 1];
        }
    }
    return sommes_p;
}

/**
 * Calcule la somme partielle des probabilités
 * Complexité spatiale et temporelle en O(n).
 * 
 * Utiliser la macro sommes_p(i, j) = sommes_p[j] - sommes_p[i]
 * pour calculer la somme partielle de i à j.
 * 
 * @param probabilites tableau des probabilités
 * @param n taille du tableau des probabilités
 * @return tableau des sommes partielles p_i + p_{i+1}
 */
double *calculer_sommes(double *probabilites, size_t n) {
    double *sommes_p = malloc(n * sizeof(double));
    double somme = 0;
    for (size_t i = 0; i < n; ++i) {
        somme += probabilites[i];
        sommes_p[i] = somme;
    }
    return sommes_p;
}