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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bellman.h"
#include "partial_probabilities.h"
#include "timing.h"
#include "utils.h"

/**
 * \fn static void lire_fichier(FILE *freqFile, int n,
                         uint64_t *probabilities, uint64_t *sommes_p)
 * \brief Lit le fichier et stocke les probabilités ainsi que les probabilités partielles.
 * \param freqFile fichier contenant les fréquences
 * \param n nombre d'éléments dans le dictionnaire, n >= 0
 */
static void lire_fichier(FILE *freqFile, int n,
                         uint64_t *probabilities, uint64_t *sommes_p) {
    assert(n > 0 && "ERROR: n must be > 0");
    assert(freqFile != NULL && "ERROR: frequencies file is a void pointer");
    uint64_t nombre;
    uint64_t somme = 0;
    for (int i = 0; i < n; ++i) {
        fscanf(freqFile, "%ld", &nombre);
        probabilities[i] = nombre;
        somme += nombre;
        sommes_p[i] = somme;
    }
    // for (int i = 0; i < n; ++i) {
    //     probabilities[i] /= (long double)somme;
    //     sommes_p[i] /= (long double)somme;
    // }
}

/**
 * build_bst
 * \fn void build_bst(int i, int j, int n, int *r, int (*bst)[2])
 * \brief Construction récusive du BST
 * \param i premier élement de la liste des racines
 * \param j dernier élement de la liste des racines
 * \param n nombre d'éléments à stocker dans l'arbre
 * \param r liste des racines
 * \param bst le Binary Search Tree
 */
void build_bst(int i, int j, int n, int *r, int (*bst)[2]) {
    int root = r(i, j, n);
    // printf("root: %i\ni: %d, j: %d\n", root, i, j);
    if (i < root) {
        bst[root][0] = r(i, root - 1, n);  // Fils gauche
        build_bst(i, root - 1, n, r, bst);
    }

    if (root < j) {
        bst[root][1] = r(root + 1, j, n);  // Fils droite
        build_bst(root + 1, j, n, r, bst);
    }
}

/**
 * print_bst
 * \fn void print_bst(int n, int *r, int (*bst)[2])
 * \brief Affichage du BST
 * \param n nombre d'éléments à stocker dans l'arbre
 * \param r liste des racines
 * \param bst le Binary Search Tree
 */
void print_bst(int n, int *r, int (*bst)[2]) {
    printf("static int BSTroot = %d;\n", r(0, n - 1, n));
    printf("static int BSTtree[%d][2] = {\n", n);
    for (int i = 0; i < n; i++) {
        printf("{%d, %d}", bst[i][0], bst[i][1]);
        if (i != n - 1) {
            printf(", \n");
        }
    }
    printf(" };\n");
}

/**
 * run_bellman_fn
 * \fn void run_bellman_fn(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n, int (*bst)[2], void (*bellman_fn)(uint64_t *, int *, uint64_t *, uint64_t *, int))
 * \brief Calcul du BST avec la fonction bellman_fn
 * \param c matrice triangulaire supérieure des coûts contenant (n * (n + 1)) / 2 éléments
 * \param r matrice triangulaire supérieure des racines contenant (n * (n + 1)) / 2 éléments
 * \param probabilities array contenant n probabilités
 * \param sommes_p array contenant n sommes partielles
 * \param n nombre d'éléments à stocker dans l'arbre
 * \param bst le Binary Search Tree
 * \param bellman_fn fonction calculant la matrice des racines et des coûts
 */
void run_bellman_fn(uint64_t *c, int *r,
                    uint64_t *probabilities, uint64_t *sommes_p,
                    int n, int (*bst)[2],
                    void (*bellman_fn)(uint64_t *, int *, uint64_t *, uint64_t *, int)) {
    (*bellman_fn)(c, r, probabilities, sommes_p, n);

    // printf("Matrice des coûts c:\n");
    // afficher_tableau_trig_uint(c, n);
    // printf("Profondeur minimale: %ld\n", c(0, n - 1, n));
    // printf("Matrice des racines r:\n");
    // afficher_tableau_trig_int(r, n);

    // Mise des coefficients du BST à -1
    memset(bst, -1, 2 * (uint64_t)n * sizeof(int));

    build_bst(0, (int)n - 1, (int)n, r, bst);  // Création de l'arbre
    print_bst(n, r, bst);
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
    int n = 0;              // Number of elements in the dictionary
    FILE *freqFile = NULL;  // File that contains n positive integers defining the relative frequence of dictinary elements

    if (argc != 3) {
        fprintf(stderr, "Usage: ./compileBST n  originalFile\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }

    {  // Conversion of parameter n in a long
        int codeRetour = EXIT_SUCCESS;
        char *posError;
        long resuLong;
        n = (int)atol(argv[1]);

        assert(argc >= 2);
        // Conversion of argv[1] en long
        resuLong = strtol(argv[1], &posError, 10);
        // Traitement des erreurs
        switch (errno) {
            case EXIT_SUCCESS:
                // Conversion du long en int
                if (resuLong > 0) {
                    n = (int)resuLong;
                    // fprintf(stderr, "Number of elements in the dictionary: %ld\n", n);
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

    uint64_t *sommes_p = malloc((uint64_t)n * sizeof(uint64_t));
    uint64_t *probabilities = malloc((uint64_t)n * sizeof(uint64_t));
    lire_fichier(freqFile, n, probabilities, sommes_p);

    // printf("Probabilities: \n");
    // afficher_tableau(probabilities, n);
    // // long double *sommes_p = calculer_sommes(probabilities, n);
    // printf("Sommes partielles: \n");
    // afficher_tableau(sommes_p, n);

    /**
     * Autre manière de stocker les sommes partielles
     * On rajoute une diagonale de zéros pour avoir C(i, i + 1) = p_i donc 
     * on construit une matrice carrée triangulaire supérieure de taille n + 1. 
     */
    // long double *sommes_p_all = calculer_sommes_all(probabilites, n + 1);
    // afficher_tableau_trig_double(sommes_p_all, n + 1);

    /**
     * On choisit de stocker seulement les coefficients utiles au calcul:
     * c est une matrice triangulaire supérieure de long double et de même pour r,
     * la matrice contenant les racines.
     * La première diagonale contient les p_i. On ne stocke pas la diagonale 
     * c(i, i) (notation de l'énoncé) ne contenant que des zéros.
     * 
     * Ainsi, dans notre programme, la diagonale c(i, i) contient les p_i.
     */
    uint64_t *c = malloc((uint64_t)(n * (n + 1)) / 2 * sizeof(uint64_t));
    int *r = malloc((uint64_t)(n * (n + 1)) / 2 * sizeof(int));
    int bst[n][2];

    /* Time benchmarks */
    t_bellman_args bellman_args = {
        .c = c,
        .r = r,
        .probabilities = probabilities,
        .sommes_p = sommes_p,
        .n = n,
    };
    // printf("\nTest bellman dik:\n");
    // bellman_fn_execution_time(&bellman_args, bellman);
    // printf("\nTest bellman ijk:\n");
    // bellman_fn_execution_time(&bellman_args, bellman_2);
    // printf("\nTest bellman jik:\n");
    // bellman_fn_execution_time(&bellman_args, bellman_3);
    // printf("\n");

    // printf("Matrice des coûts c:\n");
    // afficher_tableau_trig_double(c, n);
    // printf("Profondeur minimale: %Lf\n", c(0, n - 1, n));
    // printf("Matrice des racines r:\n");
    // afficher_tableau_trig_int(r, n);

    // // Mise des coefficients du BST à -1
    // memset(bst, -1, 2 * n * sizeof(int));

    // build_bst(0, (int)n - 1, (int)n, r, bst);  // Création de l'arbre
    // print_bst(n, r, bst);

    /* Test bellman */
    // printf("\nTest bellman dik:\n");
    // run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman);

    /* Test bellman 2 */
    // printf("\nTest bellman ijk:\n");
    // run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman_2);

    /* Test bellman 3 */
    // printf("\nTest bellman jik:\n");
    run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman_3);

    free(probabilities);
    free(sommes_p);
    // free(sommes_p_all);
    free(c);
    free(r);
    fclose(freqFile);

    return EXIT_SUCCESS;
}
