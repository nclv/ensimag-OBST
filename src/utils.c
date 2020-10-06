#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Affiche un tableau de double
 */
void afficher_tableau(double *array, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        printf("%lf - ", array[i]);
    }
    printf("\n");
}

/**
 * Affichage de la matrice triangulaire supérieure de taille n stockée dans array
 *
 * @param array tableau de taille (n * (n + 1)) / 2
 * @param n taille de la matrice carrée
 */
void afficher_tableau_trig_double(double *array, size_t n) {
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

/**
 * Affichage de la matrice triangulaire supérieure de taille n stockée dans array
 *
 * @param array tableau de taille (n * (n + 1)) / 2
 * @param n taille de la matrice carrée
 */
void afficher_tableau_trig_int(int *array, size_t n) {
    size_t line = 1;
    size_t col = 0;
    for (size_t i = 0; i < (n * (n + 1)) / 2; ++i) {
        printf("%d - ", array[i]);
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