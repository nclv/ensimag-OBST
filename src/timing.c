#include "timing.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

void bellman_fn_execution_time(t_bellman_args *bellman_args,
                               void (*bellman_fn)(uint64_t *, int *,
                                                  uint64_t *, uint64_t *, int)) {
    static clock_t start, end;
    static double cpu_time_used;

    start = clock();
    (*bellman_fn)(bellman_args->c, bellman_args->r,
                  bellman_args->probabilities, bellman_args->sommes_p,
                  bellman_args->n);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // printf("CPU time used: %lf\n", cpu_time_used);
    printf("%lf ", cpu_time_used);
}