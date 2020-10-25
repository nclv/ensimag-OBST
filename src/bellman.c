#include "bellman.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <omp.h>

#include "partial_probabilities.h"

/**
 * \fn static inline uint64_t get_min(int i, int j, int n, uint64_t *c)
 * \brief On cherche la racine optimale du sous-arbre des éléments entre i et j
 * \param i i >= 0
 * \param j j > i
 * \param n nombre d'éléments à stocker dans l'arbre, n >= 0
 * \param c matrice triangulaire supérieure contenant (n * (n + 1)) / 2 éléments
 * \return Renvoie la valeur qui minimise c(i, k) - c(k + 1, j), i <= k < j
 */
static inline uint64_t get_min(int i, int j, int n, uint64_t *c, int *r) {
    uint64_t min = UINT_MAX;
    uint64_t temp;
    int argmin = r(i, j - 1, n);
    // on utilise des int car k - 1 peut être négatif
    // Knuth, 1971
    // for (int k = i; k <= j; ++k) {
    #pragma omp simd reduction(min:min)
    for (int k = argmin; k <= r(i + 1, j, n); ++k) {
        // printf("c(%i, %i) + c(%i, %i) = %.2f + %.2f\n", i, k - 1,
        //        k + 1, j, c(i, k - 1, n), c(k + 1, j, n));
        /** 
        * Attention aux cas limites pour i == k et j == k
        * si i == k: temp = c(k + 1, j, n)
        * si j == k: temp = c(i, k - 1, n)
        */
        temp = ((i < k) ? c(i, k - 1, n) : 0) + ((j > k) ? c(k + 1, j, n) : 0);
        if (temp < min) {
            min = temp;
            argmin = k;
            // printf("min: %.17f, r: %i\n", min, argmin);
        }
    }
    r(i, j, n) = argmin;
    return min;
}

/**
 * \fn static void bellman(uint64_t *c, uint64_t *probabilities, uint64_t *sommes_p, int n)
 * \brief Remplie la matrice triangulaire supérieure des coefficients, c, avec l'algorithme de Bellman
 * \param c matrice triangulaire supérieure des coûts contenant (n * (n + 1)) / 2 éléments
 * \param r matrice triangulaire supérieure des racines contenant (n * (n + 1)) / 2 éléments
 * \param probabilities array contenant n probabilités
 * \param sommes_p array contenant n sommes partielles
 * \param n nombre d'éléments à stocker dans l'arbre
 */
void bellman(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n) {
    /* Initialisation de la diagonale inférieure c(i, i) avec les p_i */
    for (int i = 0; i < n; ++i) {
        c(i, i, n) = probabilities[i];
        r(i, i, n) = (int)i;
        // printf("c(%i, %i) = %.2ld\n", i, i, c(i, i, n));
    }

    /** 
     * Calcul des coefficients c(i, j) pour 0 <= i < j < n 
     * On itère diagonale par diagonale en commençant par celle directement
     * au dessus de la diagonale des probabilités.
     * 
     */
    uint64_t min;
    int j;
    // création de threads en parallèle
    #pragma omp parallel default(shared)
    for (int diagonal = 1; diagonal < n; ++diagonal) {
        // on distribue la boucle sur i parmis les threads
        #pragma omp for 
        for (int i = 0; i < n - diagonal; ++i) {
            j = i + diagonal;
            /* Récupération de la valeur qui minimise c(i, k) - c(k + 1, j), i <= k < j */
            min = get_min((int)i, (int)j, (int)n, c, r);
            // printf("min: %.2f\n", min);
            /** 
            * Notre précalcul des sommes partielles des probabilités nous oblige à 
            * considérer 2 cas, en effet sommes_p[j] contient la sommes des probabilités 
            * de p_0 à p_j inclu:
            * 
            * si i == 0 (ie. on veut la somme de 0 inclu à j inclu): on prend sommes_p[j]
            * sinon: on prend sommes_p[j] - sommes_p[i - 1] (ici avec une macro)
            */
            if (i > 0) {
                // printf("sommes_p[%li] - sommes_p[%li]: %.2f\n", j, i, sommes_p(i - 1, j));
                // printf("= %.2f - %.2f\n", sommes_p[j], sommes_p[i - 1]);
                c(i, j, n) = min + sommes_p(i - 1, j);
            } else {
                // printf("sommes_p[%li]: %.2f\n", j, sommes_p[j]);
                c(i, j, n) = min + sommes_p[j];
            }
            // printf("c(%li, %li) = %.2f\n", i, j, c(i, j, n));
        } // barrière implicite, tous les threads attendent la fin de la boucle avant de passer à la diagonale suivante
    } // fin de la boucle parallèle
}

void bellman_2(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n) {
    uint64_t min;
    for (int i = n - 1; i != -1; --i) {
        c(i, i, n) = probabilities[i];
        r(i, i, n) = i;
        for (int j = i + 1; j < n; ++j) {
            min = get_min(i, j, n, c, r);
            if (i > 0) {
                c(i, j, n) = min + sommes_p(i - 1, j);
            } else {
                c(i, j, n) = min + sommes_p[j];
            }
        }
    }
}

void bellman_3(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n) {
    uint64_t min;
    for (int j = 0; j < n; ++j) {
        c(j, j, n) = probabilities[j];
        r(j, j, n) = j;
        for (int i = ((j != 0) ? j - 1 : -1); i != -1; --i) {
            min = get_min(i, j, n, c, r);
            if (i > 0) {
                c(i, j, n) = min + sommes_p(i - 1, j);
            } else {
                c(i, j, n) = min + sommes_p[j];
            }
        }
    }
}