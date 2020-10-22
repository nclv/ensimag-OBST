#include "utils.h"

#include <stdint.h>
#include <stdio.h>

/**
 * Affiche un tableau de double
 */
void afficher_tableau(long double *array, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%Lf - ", array[i]);
    }
    printf("\n");
}

/**
 * Affichage de la matrice triangulaire supérieure de taille n stockée dans array
 *
 * @param array tableau de taille (n * (n + 1)) / 2
 * @param n taille de la matrice carrée
 */
void afficher_tableau_trig_double(long double *array, int n) {
    int line = 1;
    int col = 0;
    for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
        printf("%Lf - ", array[i]);
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
void afficher_tableau_trig_uint(uint64_t *array, int n) {
    int line = 1;
    int col = 0;
    for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
        printf("%ld - ", array[i]);
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
void afficher_tableau_trig_int(int *array, int n) {
    int line = 1;
    int col = 0;
    for (int i = 0; i < (n * (n + 1)) / 2; ++i) {
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
