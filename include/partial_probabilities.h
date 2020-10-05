#ifndef partial_probabilities_H_
#define partial_probabilities_H_

#include <stddef.h>

/** 
 * La valeur à l'emplacement (i, j) dans la matrice triangulaire supérieure se trouve 
 * dans la mémoire à l'emplacement (i * (2 * (n - 1) - i + 1)) / 2 + j.
*/
#define sommes_p_trig(i, j, n) (sommes_p[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])
#define c(i, j, n) (c[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])
#define sommes_p(i, j) (sommes_p[j] - sommes_p[i])

extern double *calculer_sommes_all(double *probabilites, size_t n);
extern double *calculer_sommes(double *probabilites, size_t n);

#endif