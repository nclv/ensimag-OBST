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

#include "partial_probabilities.h"
#include "timing.h"
#include "utils.h"

#define c(i, j, n) (c[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])
#define r(i, j, n) (r[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])

/**
 * \fn static void lire_fichier(FILE *freqFile, size_t n,
                         long double *probabilities, long double *sommes_p)
 * \brief Lit le fichier et stocke les probabilités ainsi que les probabilités partielles.
 * \param freqFile fichier contenant les fréquences
 * \param n nombre d'éléments dans le dictionnaire, n >= 0
 */
static void lire_fichier(FILE *freqFile, size_t n,
                         long double *probabilities, long double *sommes_p) {
    assert(n > 0 && "ERROR: n must be > 0");
    assert(freqFile != NULL && "ERROR: frequencies file is a void pointer");
    long nombre;
    long somme = 0;
    for (size_t i = 0; i < n; ++i) {
        fscanf(freqFile, "%ld", &nombre);
        probabilities[i] = (long double)nombre;
        somme += nombre;
        sommes_p[i] = (long double)somme;
    }
    for (size_t i = 0; i < n; ++i) {
        probabilities[i] /= (long double)somme;
        sommes_p[i] /= (long double)somme;
    }
}

/**
 * \fn static inline long double get_min(int i, int j, int n, long double *c)
 * \brief On cherche la racine optimale du sous-arbre des éléments entre i et j
 * \param i i >= 0
 * \param j j > i
 * \param n nombre d'éléments à stocker dans l'arbre, n >= 0
 * \param c matrice triangulaire supérieure contenant (n * (n + 1)) / 2 éléments
 * \return Renvoie la valeur qui minimise c(i, k) - c(k + 1, j), i <= k < j
 */
static inline long double get_min(int i, int j, int n, long double *c, int *r) {
    long double min = __FLT_MAX__;
    long double temp;
    int argmin = r(i, j - 1, n);
    // on utilise des int car k - 1 peut être négatif
    // Knuth, 1971
    // for (int k = i; k <= j; ++k) {
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
 * \fn static void bellman(long double *c, long double *probabilities, long double *sommes_p, size_t n)
 * \brief Remplie la matrice triangulaire supérieure des coefficients, c, avec l'algorithme de Bellman
 * \param c matrice triangulaire supérieure des coûts contenant (n * (n + 1)) / 2 éléments
 * \param r matrice triangulaire supérieure des racines contenant (n * (n + 1)) / 2 éléments
 * \param probabilities array contenant n probabilités
 * \param sommes_p array contenant n sommes partielles
 * \param n nombre d'éléments à stocker dans l'arbre
 */
static void bellman(long double *c, int *r, long double *probabilities, long double *sommes_p, size_t n) {
    /* Initialisation de la diagonale inférieure c(i, i) avec les p_i */
    for (size_t i = 0; i < n; ++i) {
        c(i, i, n) = probabilities[i];
        r(i, i, n) = (int)i;
        // printf("c(%li, %li) = %.2f\n", i, i, c(i, i, n));
    }

    /** 
     * Calcul des coefficients c(i, j) pour 0 <= i < j < n 
     * On itère diagonale par diagonale en commençant par celle directement
     * au dessus de la diagonale des probabilités.
     * 
     */
    long double min;
    size_t j;
    for (size_t diagonal = 1; diagonal < n; ++diagonal) {
        for (size_t i = 0; i < n - diagonal; ++i) {
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
        }
    }
}

static void bellman_2(long double *c, int *r, long double *probabilities, long double *sommes_p, size_t n) {
    long double min;
    for (int i = n - 1; i >= 0; --i) {
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

static void bellman_3(long double *c, int *r, long double *probabilities, long double *sommes_p, size_t n) {
    long double min;
    for (int j = 0; j < n; ++j) {
        c(j, j, n) = probabilities[j];
        r(j, j, n) = j;
        for (int i = j - 1; i >= 0; --i) {
            min = get_min(i, j, n, c, r);
            if (i > 0) {
                c(i, j, n) = min + sommes_p(i - 1, j);
            } else {
                c(i, j, n) = min + sommes_p[j];
            }
        }
    }
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
    printf("static int BSTtree[%ld][2] = {\n", n);
    for (size_t i = 0; i < n; i++) {
        printf("{%d, %d}", bst[i][0], bst[i][1]);
        if (i < n - 1) {
            printf(", \n");
        }
    }
    printf(" };\n");
}

/**
 * run_bellman_fn
 * \fn void run_bellman_fn(long double *c, int *r, long double *probabilities, long double *sommes_p, size_t n, int (*bst)[2], void (*bellman_fn)(long double *, int *, long double *, long double *, size_t))
 * \brief Calcul du BST avec la fonction bellman_fn
 * \param c matrice triangulaire supérieure des coûts contenant (n * (n + 1)) / 2 éléments
 * \param r matrice triangulaire supérieure des racines contenant (n * (n + 1)) / 2 éléments
 * \param probabilities array contenant n probabilités
 * \param sommes_p array contenant n sommes partielles
 * \param n nombre d'éléments à stocker dans l'arbre
 * \param bst le Binary Search Tree
 * \param bellman_fn fonction calculant la matrice des racines et des coûts
 */
void run_bellman_fn(long double *c, int *r,
                    long double *probabilities, long double *sommes_p,
                    size_t n, int (*bst)[2],
                    void (*bellman_fn)(long double *, int *, long double *, long double *, size_t)) {
    (*bellman_fn)(c, r, probabilities, sommes_p, n);

    // printf("Matrice des coûts c:\n");
    // afficher_tableau_trig_double(c, n);
    // printf("Profondeur minimale: %f\n", c(0, n - 1, n));
    // printf("Matrice des racines r:\n");
    // afficher_tableau_trig_int(r, n);

    // Mise des coefficients du BST à -1
    memset(bst, -1, 2 * n * sizeof(int));

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

    long double *sommes_p = malloc(n * sizeof(long double));
    long double *probabilities = malloc(n * sizeof(long double));
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
    long double *c = malloc((n * (n + 1)) / 2 * sizeof(long double));
    int *r = malloc((n * (n + 1)) / 2 * sizeof(int));
    int bst[n][2];


    /* Time benchmarks */
    t_bellman_args bellman_args = {
        .c=c,
        .r=r,
        .probabilities=probabilities,
        .sommes_p=sommes_p,
        .n=n,
    };
    // printf("\nTest bellman dik:\n");
    bellman_fn_execution_time(&bellman_args, bellman);
    // printf("\nTest bellman ijk:\n");
    // bellman_fn_execution_time(&bellman_args, bellman_2);
    // printf("\nTest bellman jik:\n");
    // bellman_fn_execution_time(&bellman_args, bellman_3);
    // printf("\n");

    printf("Matrice des coûts c:\n");
    afficher_tableau_trig_double(c, n);
    printf("Profondeur minimale: %Lf\n", c(0, n - 1, n));
    printf("Matrice des racines r:\n");
    afficher_tableau_trig_int(r, n);

    // Mise des coefficients du BST à -1
    memset(bst, -1, 2 * n * sizeof(int));

    build_bst(0, (int)n - 1, (int)n, r, bst);  // Création de l'arbre
    print_bst(n, r, bst);

    /* Test bellman */
    // printf("\nTest bellman dik:\n");
    // run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman);

    /* Test bellman 2 */
    // printf("\nTest bellman ijk:\n");
    // run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman_2);

    /* Test bellman 3 */
    // printf("\nTest bellman jik:\n");
    // run_bellman_fn(c, r, probabilities, sommes_p, n, bst, bellman_3);

    free(probabilities);
    free(sommes_p);
    // free(sommes_p_all);
    free(c);
    free(r);
    fclose(freqFile);

    return EXIT_SUCCESS;
}
