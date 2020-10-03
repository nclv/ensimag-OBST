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

#define sommes_p(i, j) (sommes_p[j] - sommes_p[i])

typedef struct BST {
	int BSTroot;
	int **BSTtree;
} BST;

double *calculer_sommes(double *probabilites, long taille);
double *lire_fichier(FILE* freqFile, long taille);
void afficher_tableau(double* tableau, long taille);

/**
 * Calcule la somme des probabilités et utilise la macro
 * sommes_p(i, j) pour calculer la somme partielle de i à j
 */
double *calculer_sommes(double *probabilites, long taille) {
	double *sommes_p = malloc(taille * sizeof(double));
	double somme = 0;
	for (long i = 0; i < taille; ++i) {
		somme += probabilites[i];
		sommes_p[i] = somme;
	}
	return sommes_p;
}

/**
 * Lit le fichier et stocke les probabilités associées
 */
double *lire_fichier(FILE* freqFile, long taille) {
	long somme = 0;
	long nombre;
	double *probabilites = malloc(taille * sizeof(double));
	for (long i = 0; i < taille; ++i) {
		fscanf(freqFile, "%ld", &nombre);
		probabilites[i] = (double) nombre;
		somme += nombre;
	}
	for (long i = 0; i < taille; ++i) {
		probabilites[i] /= (double) somme;
	}
	return probabilites;
}

/**
 * Affiche un tableau de double
 */
void afficher_tableau(double* tableau, long taille) {
	for (long i = 0; i < taille - 1; i++) {
		printf("%lf - ", tableau[i]);
	}
	printf("%lf\n", tableau[taille - 1]);
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
    long n = 0;             // Number of elements in the dictionary
    FILE *freqFile = NULL;  // File that contains n positive integers defining the relative frequence of dictinary elements

    if (argc != 3) {
        fprintf(stderr, "!!!!! Usage: ./compileBST n  originalFile !!!!!\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }

    {  // Conversion of parameter n in a long
        int codeRetour = EXIT_SUCCESS;
        char *posError;
        long resuLong;
        n = atol(argv[1]);

        assert(argc >= 2);
        // Conversion of argv[1] en long
        resuLong = strtol(argv[1], &posError, 10);
        // Traitement des erreurs
        switch (errno) {
            case EXIT_SUCCESS:
                // Conversion du long en int
                if (resuLong > 0) {
                    n = (long)resuLong;
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

    freqFile = fopen(argv[2], "r");
    if (freqFile == NULL) {
        fprintf(stderr, "!!!!! Error opening originalFile !!!!!\n");
        exit(EXIT_FAILURE);
    }

	//BEGIN
	double* probabilites = lire_fichier(freqFile, n);
	afficher_tableau(probabilites, n);
	double* sommes_p = calculer_sommes(probabilites, n);
	afficher_tableau(sommes_p, n);
	printf("%lf\n", sommes_p(2, 3));
	//END


    fclose(freqFile);

    return 0;
}
