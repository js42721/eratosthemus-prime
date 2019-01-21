#include <inttypes.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "sieve.h"
#include "types.h"

#define SEGMENT_SIZE (1 << 18) /* [1, 2^29] */

int main(int argc, char **argv)
{
    u64 lower;
    u64 upper;
    u32 threads;
    u64 result;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s lower upper [threads]\n", argv[0]);
        return EXIT_FAILURE;
    }

    lower = upper = 0;
    threads = omp_get_num_procs();

    sscanf(argv[1], "%"SCNu64, &lower);
    sscanf(argv[2], "%"SCNu64, &upper);

    if (argc > 3)
        sscanf(argv[3], "%"SCNu32, &threads);

    result = sieve_count(lower, upper, SEGMENT_SIZE, threads);

    printf("%"PRIu64"\n", result);

    return 0;
}
