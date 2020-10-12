#ifndef _TIMING_
#define _TIMING_

#include <stdlib.h>

typedef struct bellman_args {
    double *c;
    int *r;
    double *probabilities;
    double *sommes_p;
    size_t n;
} t_bellman_args;

extern void bellman_fn_execution_time(t_bellman_args *bellman_args,
                                      void (*bellman_fn)(double *, int *,
                                                         double *, double *, size_t));

#endif