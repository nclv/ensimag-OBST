#ifndef _TIMING_
#define _TIMING_

#include <stdlib.h>

typedef struct bellman_args {
    long double *c;
    int *r;
    long double *probabilities;
    long double *sommes_p;
    size_t n;
} t_bellman_args;

extern void bellman_fn_execution_time(t_bellman_args *bellman_args,
                                      void (*bellman_fn)(long double *, int *,
                                                         long double *, long double *, size_t));

#endif