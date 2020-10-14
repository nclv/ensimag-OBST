#ifndef _TIMING_
#define _TIMING_

#include <stdint.h>

typedef struct bellman_args {
    uint64_t *c;
    int *r;
    uint64_t *probabilities;
    uint64_t *sommes_p;
    int n;
} t_bellman_args;

extern void bellman_fn_execution_time(t_bellman_args *bellman_args,
                                      void (*bellman_fn)(uint64_t *, int *,
                                                         uint64_t *, uint64_t *, int));

#endif