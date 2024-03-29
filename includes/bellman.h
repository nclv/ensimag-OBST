#ifndef _BELLMAN_
#define _BELLMAN_

#include <stdint.h>

// ligne par ligne
#define c(i, j, n) (c[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])
#define r(i, j, n) (r[((i) * (2 * ((n)-1) - (i) + 1)) / 2 + (j)])

// diagonale par diagonale
// #define c(i, j, n) (c[((i) - (((i) - (j)) * (2 * (n) + (i) - (j) + 1)) / 2)])
// #define r(i, j, n) (r[((i) - (((i) - (j)) * (2 * (n) + (i) - (j) + 1)) / 2)])

extern void bellman(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n);
extern void bellman_2(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n);
extern void bellman_3(uint64_t *c, int *r, uint64_t *probabilities, uint64_t *sommes_p, int n);

#endif