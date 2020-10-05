/*! \file compileBST.c
 *  \brief	   This implements the applyPatch program.
 *  \author    Lucie Pansart
 *  \author    Jean-Louis Roch
 *  \version   1.0
 *  \date      30/9/2016
 *  \warning   Usage: compileBST n originalFile 
 *  \copyright GNU Public License.
 */

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "optimalBST.h"
#include "partial_probabilities.h"
#include "utils.h"

/**
 * lire_fichier
 * Lit le fichier et stocke les probabilités ainsi que 
 * les probabilités partielles.
 * 
 * \param freqFile fichier contenant les fréquences
 * \param n nombre d'éléments dans le dictionnaire
 */
static void lire_fichier(FILE *freqFile, size_t n,
                         double *probabilities, double *sommes_p) {
    assert(n > 0 && "ERROR: n must be > 0");
    assert(freqFile != NULL && "ERROR: frequencies file is a void pointer");
    long nombre;
    long somme = 0;
    for (size_t i = 0; i < n; ++i) {
        fscanf(freqFile, "%ld", &nombre);
        probabilities[i] = (double)nombre;
        somme += nombre;
        sommes_p[i] = (double)somme;
    }
    for (size_t i = 0; i < n; ++i) {
        probabilities[i] /= (double)somme;
        sommes_p[i] /= (double)somme;
    }
}

/**
 * Renvoie la valeur de k qui minimise c(i, k) - c(k + 1, j), i <= k < j
 * 
 * @param i
 * @param j
 * @param n
 * @param c
 */ 
static inline double get_min(size_t i, size_t j, size_t n, double *c) {
    double min = __FLT_MAX__;
    for (size_t k = i; k < j; ++k) {
        printf("c(%li, %li) + c(%li, %li) = %.2f + %.2f\n", i, k, k + 1, j, c(i, k, n), c(k + 1, j, n));
        double temp = c(i, k, n) + c(k + 1, j, n);
        if (temp < min) {
            min = temp;
        }
    }
    return min;
}

static void bellman(double *c, double *probabilities, double *sommes_p, size_t n) {
    /* Initialisation de la diagonale à p_0 */
    for (size_t i = 0; i < n; ++i) {
        c(i, i, n) = probabilities[i];
        printf("c(%li, %li) = %.2f\n", i, i, c(i, i, n));
    }
    size_t j;
    /* Calcul des sommes des probabilités pour i > j */
    for (size_t diagonal = 1; diagonal < n; ++diagonal) {
        for (size_t i = 0; i < n - diagonal; ++i) {
            j = i + diagonal;
            double min = get_min(i, j, n, c);
            printf("min: %.2f\n", min);
            if (i != 0) {
                printf("somme_%li%li: %.2f\n", i, j, sommes_p(i, j));
                c(i, j, n) = min + sommes_p(i, j);
            } else {
                printf("somme_%li%li: %.2f\n", i, j, sommes_p[j - 1]);
                c(i, j, n) = min + sommes_p[j - 1];
            }
            printf("c(%li, %li) = %.2f\n", i, j, c(i, j, n));
        }
    }
}

/**
 * Main function
 * \brief Main function
 * \param argc  A count of the number of command-line arguments
 * \param argv  An argument vector of the command-line arguments.
 * \warning Must be called with a positive long integer, n,  and a filename, freqFile, as commandline parameters and in the given order.
 * \returns { 0 if succeeds and prints C code implementing an optimal ABR on stdout; exit code otherwise}
 */
int main(int argc, char *argv[]) {
    size_t n = 0;           // Number of elements in the dictionary
    FILE *freqFile = NULL;  // File that contains n positive integers defining the relative frequence of dictinary elements

    if (argc != 3) {
        fprintf(stderr, "Usage: ./compileBST n  originalFile\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }

    {  // Conversion of parameter n in a long
        int codeRetour = EXIT_SUCCESS;
        char *posError;
        long resuLong;
        n = (size_t)atol(argv[1]);

        assert(argc >= 2);
        // Conversion of argv[1] en long
        resuLong = strtol(argv[1], &posError, 10);
        // Traitement des erreurs
        switch (errno) {
            case EXIT_SUCCESS:
                // Conversion du long en int
                if (resuLong > 0) {
                    n = (size_t)resuLong;
                    fprintf(stderr, "Number of elements in the dictionary: %ld\n", n);
                } else {
                    (void)fprintf(stderr, "%s cannot be converted into a positive integer matching the number of elements in the dicionary.\n", argv[1]);
                    codeRetour = EXIT_FAILURE;
                }
                break;

            case EINVAL:
                perror(__func__);
                (void)fprintf(stderr, "%s does not match a long integer value. \n", argv[1]);
                codeRetour = EXIT_FAILURE;
                break;

            case ERANGE:
                perror(__func__);
                (void)fprintf(stderr,
                              "%s does not fit in a long int :\n"
                              "out of bound [%ld;%ld]\n",
                              argv[1], LONG_MIN, LONG_MAX);
                codeRetour = EXIT_FAILURE;
                break;
            default:
                perror(__func__);
                codeRetour = EXIT_FAILURE;
        }  // switch (errno)
        if (codeRetour != EXIT_SUCCESS) return codeRetour;
    }

    /* Ouverture sécurisée du fichier */
    freqFile = fopen(argv[2], "r");
    if (freqFile == NULL) {
        fprintf(stderr, "ERROR: cannot open file '%s': %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }


    double *sommes_p = malloc(n * sizeof(double));
    double *probabilities = malloc(n * sizeof(double));

    lire_fichier(freqFile, n, probabilities, sommes_p);
    printf("Probabilities: \n");
    afficher_tableau(probabilities, n);
    // double *sommes_p = calculer_sommes(probabilities, n);
    printf("Sommes partielles: \n");
    afficher_tableau(sommes_p, n);
    // printf("%lf\n", sommes_p(2, 3));

    // int abr[n][2];
    double *c = malloc((n * (n + 1)) / 2 * sizeof(double));
    // double *c = calculer_sommes_all(probabilities, n);
    // printf("Matrice C:\n");
    // afficher_tableau_all(c, n);
    bellman(c, probabilities, sommes_p, n);
    printf("Matrice C:\n");
    afficher_tableau_all(c, n);
    printf("Profondeur minimale: %f\n", c(0, n - 1, n));

    /**
     * On rajoute une diagonale de zéros pour avoir C(i, i + 1) = p_i donc 
     * on construit une matrice carrée triangulaire supérieure de taille n + 1. 
     */
    // double *sommes_p_all = calculer_sommes_all(probabilites, n + 1);
    // afficher_tableau_all(sommes_p_all, n + 1);

    printf("static int BSTroot = ...;\n");
    printf("static int BSTtree[<max_values>][2] = {");
    printf(" };\n");

    free(probabilities);
    free(sommes_p);
    // free(sommes_p_all);
    fclose(freqFile);

    return EXIT_SUCCESS;
}
