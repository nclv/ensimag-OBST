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
 * Lit le fichier et stocke les probabilités ainsi que 
 * les probabilités partielles.
 * 
 * @param freqFile fichier contenant les fréquences
 * @param n nombre d'éléments dans le dictionnaire
 */
static void lire_fichier(FILE *freqFile, size_t n,
                         double *probabilities, double *sommes_p) {
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
    double *probabilites = malloc(n * sizeof(double));

    lire_fichier(freqFile, n, probabilites, sommes_p);
    printf("Probabilities: \n");
    afficher_tableau(probabilites, n);
    // double *sommes_p = calculer_sommes(probabilites, n);
    printf("Sommes partielles: \n");
    afficher_tableau(sommes_p, n);
    // printf("%lf\n", sommes_p(2, 3));

    // int abr[n][2];

    /**
     * On rajoute une diagonale de zéros pour avoir C(i, i + 1) = p_i donc 
     * on construit une matrice carrée triangulaire supérieure de taille n + 1. 
     */
    // double *sommes_p_all = calculer_sommes_all(probabilites, n + 1);
    // afficher_tableau_all(sommes_p_all, n + 1);

    printf("static int BSTroot = ...;\n");
    printf("static int BSTtree[<max_values>][2] = {");
    printf(" };\n");

    free(probabilites);
    free(sommes_p);
    // free(sommes_p_all);
    fclose(freqFile);

    return EXIT_SUCCESS;
}
